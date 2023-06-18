// Fill out your copyright notice in the Description page of Project Settings.


#include "LagCompensationComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/Blaster.h"
#include "DrawDebugHelpers.h"

// Lag Compensation ���� ����� ������
// �ֿ� ���� ��� ƽ�� �����ϵ��� ����
ULagCompensationComponent::ULagCompensationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// ���� ���� �� ȣ��Ǵ� �Լ�
void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();  // �θ� Ŭ������ BeginPlay �Լ� ȣ��
}

// �� ������ ���� ������ �����ϴ� �Լ�
// �� �Լ��� �� ������ ���� ����
FFramePackage ULagCompensationComponent::InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime)
{
	// �� ������ ���� �ð� �Ÿ� ���
	const float Distance = YoungerFrame.Time - OlderFrame.Time;

	// ���� ���� ���
	// Ŭ���� �Լ��� ����Ͽ� ���� ������ 0�� 1 ���̿� �ӹ������� ��
	const float InterpFraction = FMath::Clamp((HitTime - OlderFrame.Time) / Distance, 0.f, 1.f);

	// ���� ������ ��Ű�� ����
	FFramePackage InterpFramePackage;
	InterpFramePackage.Time = HitTime; // ���� ������ ��Ű���� �ð��� Ÿ�� �ð����� ����

	// �� �浹 ���ڿ� ���� ����
	for (auto& YoungerPair : YoungerFrame.HitBoxInfo)
	{
		const FName& BoxInfoName = YoungerPair.Key;

		const FBoxInformation& OlderBox = OlderFrame.HitBoxInfo[BoxInfoName];
		const FBoxInformation& YoungerBox = YoungerFrame.HitBoxInfo[BoxInfoName];

		FBoxInformation InterpBoxInfo;

		// ��ġ�� ȸ���� ���� ���� ����
		InterpBoxInfo.Location = FMath::VInterpTo(OlderBox.Location, YoungerBox.Location, 1.f, InterpFraction);
		InterpBoxInfo.Rotation = FMath::RInterpTo(OlderBox.Rotation, YoungerBox.Rotation, 1.f, InterpFraction);

		// ũ��� ���� ������ ũ�⸦ ���
		InterpBoxInfo.BoxExtent = YoungerBox.BoxExtent;

		// ��� ���� ���ڸ� ��Ű���� �߰�
		InterpFramePackage.HitBoxInfo.Add(BoxInfoName, InterpBoxInfo);
	}

	// ��� ���� ������ ��Ű�� ��ȯ
	return InterpFramePackage;
}

// ������ �����ߴ��� Ȯ���ϴ� �Լ�
// ��Ű��, ������ ĳ����, Ʈ���̽� ���� ��ġ, ���� ��ġ�� �μ��� ����
FServerSideRewindResult ULagCompensationComponent::ConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation)
{
	if (HitCharacter == nullptr) return FServerSideRewindResult(); // ������ ĳ���Ͱ� ������ ��� ��ȯ

	// ���� ������ ���� �� ���� ��ġ ĳ��
	FFramePackage CurrentFrame;
	CacheBoxPositions(HitCharacter, CurrentFrame);

	// ���� �̵�
	MoveBoxes(HitCharacter, Package);

	// ĳ���� �޽� �浹 ��Ȱ��ȭ
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// �Ӹ� �浹�� ���� Ȱ��ȭ
	UBoxComponent* HeadBox = HitCharacter->HitCollisionBoxes[FName("head")];
	HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);

	// ���� Ȯ���� ���� ��Ʈ ��� ����
	FHitResult ConfirmHitResult;

	// Ʈ���̽� �� ���� ���
	const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;

	UWorld* World = GetWorld();
	if (World)
	{
		// ���� Ʈ���̽��� �����Ͽ� ���� Ȯ��
		World->LineTraceSingleByChannel(
			ConfirmHitResult,
			TraceStart,
			TraceEnd,
			ECC_HitBox
		);
		if (ConfirmHitResult.bBlockingHit) // �Ӹ��� ����ٸ� ���� ��ȯ
		{
			ResetHitBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, true };
		}
		else // �Ӹ��� �� ����ٸ� �ٸ� ���ڵ� Ȯ��
		{
			for (auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
			{
				if (HitBoxPair.Value != nullptr)
				{
					HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
				}
			}
			World->LineTraceSingleByChannel(
				ConfirmHitResult,
				TraceStart,
				TraceEnd,
				ECC_HitBox
			);
			if (ConfirmHitResult.bBlockingHit)
			{
				ResetHitBoxes(HitCharacter, CurrentFrame);
				EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
				return FServerSideRewindResult{ true, false };
			}
		}
	}

	// �浹�� �����ٸ� ���ڵ��� �����ϰ� �޽� �浹�� �ٽ� Ȱ��ȭ�� �� ��� ��ȯ
	ResetHitBoxes(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
	return FServerSideRewindResult{ false, false };
}


// �� ���� ��ҿ��� �߻�ü�� ������ Ȯ���ϴ� �޼���
// �ش� �޼���� �־��� ������ ��Ű���� ĳ����, �߻� ��ġ, �ʱ� �ӵ�, ���� �ð��� �̿��Ͽ� 
// �߻�ü�� ĳ���͸� �����ߴ����� Ȯ���ϰ�, �� ����� ��ȯ�Ѵ�.
FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	// ���� ĳ������ ��Ʈ�ڽ� ��ġ�� ĳ��
	FFramePackage CurrentFrame;
	CacheBoxPositions(HitCharacter, CurrentFrame);

	// ���� ������ ��Ű���� ���·� ��Ʈ�ڽ��� �̵�
	MoveBoxes(HitCharacter, Package);

	// ĳ������ �޽� �浹�� ��Ȱ��ȭ
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// �Ӹ� ��Ʈ�ڽ��� �浹�� Ȱ��ȭ
	UBoxComponent* HeadBox = HitCharacter->HitCollisionBoxes[FName("head")];
	HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);

	// �߻�ü ��� ������ ���� �Ķ���� ����
	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithCollision = true;
	PathParams.MaxSimTime = MaxRecordTime;
	PathParams.LaunchVelocity = InitialVelocity;
	PathParams.StartLocation = TraceStart;
	PathParams.SimFrequency = 15.f;
	PathParams.ProjectileRadius = 5.f;
	PathParams.TraceChannel = ECC_HitBox;
	PathParams.ActorsToIgnore.Add(GetOwner());

	FPredictProjectilePathResult PathResult;

	// �߻�ü ��θ� �����Ͽ� �Ӹ��� �����ߴ��� Ȯ��
	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
	if (PathResult.HitResult.bBlockingHit) // �Ӹ��� �������� ���, �̸� ����
	{
		ResetHitBoxes(HitCharacter, CurrentFrame);
		EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
		return FServerSideRewindResult{ true, true };
	}
	else // �Ӹ��� �������� �ʾ��� ���, �ٸ� ��Ʈ�ڽ��� Ȯ��
	{
		for (auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
		{
			if (HitBoxPair.Value != nullptr)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			}
		}

		// �ٸ� ��Ʈ�ڽ��� �����ߴ��� Ȯ��
		UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
		if (PathResult.HitResult.bBlockingHit)
		{
			ResetHitBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, false };
		}
	}

	// �������� �ʾ��� ���, ��Ʈ�ڽ��� �����ϰ� �޽� �浹�� �ٽ� Ȱ��ȭ
	ResetHitBoxes(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
	return FServerSideRewindResult{ false, false };
}


// �� ���� ��ҿ��� ������ ������ Ȯ���ϴ� �޼����Դϴ�.
// �ش� �޼���� �־��� ������ ��Ű����, �߻� ���� ��ġ, ���� ��ġ���� �̿��Ͽ� 
// ������ �� ĳ���͸� �����ߴ����� Ȯ���ϰ�, �� ����� ��ȯ�մϴ�.
FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations)
{
	// ������ ��Ű���� ĳ���͵��� ��ȿ���� Ȯ��
	for (auto& Frame : FramePackages)
	{
		if (Frame.Character == nullptr) return FShotgunServerSideRewindResult();
	}

	FShotgunServerSideRewindResult ShotgunResult;
	TArray<FFramePackage> CurrentFrames;

	// �� ĳ������ ��Ʈ�ڽ� ��ġ�� ĳ���ϰ� ���� ������ ��Ű���� ���·� �̵�
	for (auto& Frame : FramePackages)
	{
		FFramePackage CurrentFrame;
		CurrentFrame.Character = Frame.Character;
		CacheBoxPositions(Frame.Character, CurrentFrame);
		MoveBoxes(Frame.Character, Frame);
		EnableCharacterMeshCollision(Frame.Character, ECollisionEnabled::NoCollision);
		CurrentFrames.Add(CurrentFrame);
	}

	// �� ĳ������ �Ӹ� ��Ʈ�ڽ��� �浹�� Ȱ��ȭ
	for (auto& Frame : FramePackages)
	{
		UBoxComponent* HeadBox = Frame.Character->HitCollisionBoxes[FName("head")];
		HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
	}

	UWorld* World = GetWorld();

	// �Ӹ� ���� Ȯ���ϱ� ���� ���� Ʈ���̽��� ����
	for (auto& HitLocation : HitLocations)
	{
		FHitResult ConfirmHitResult;
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;
		if (World)
		{
			World->LineTraceSingleByChannel(
				ConfirmHitResult,
				TraceStart,
				TraceEnd,
				ECC_HitBox
			);
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ConfirmHitResult.GetActor());
			if (BlasterCharacter)
			{
				if (ShotgunResult.HeadShots.Contains(BlasterCharacter))
				{
					ShotgunResult.HeadShots[BlasterCharacter]++;
				}
				else
				{
					ShotgunResult.HeadShots.Emplace(BlasterCharacter, 1);
				}
			}
		}
	}

	// ��� ��Ʈ�ڽ��� �浹�� Ȱ��ȭ�ϰ�, �Ӹ� ��Ʈ�ڽ��� �浹�� ��Ȱ��ȭ
	for (auto& Frame : FramePackages)
	{
		for (auto& HitBoxPair : Frame.Character->HitCollisionBoxes)
		{
			if (HitBoxPair.Value != nullptr)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			}
		}
		UBoxComponent* HeadBox = Frame.Character->HitCollisionBoxes[FName("head")];
		HeadBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// ���� ���� Ȯ���ϱ� ���� ���� Ʈ���̽��� ����
	for (auto& HitLocation : HitLocations)
	{
		FHitResult ConfirmHitResult;
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;
		if (World)
		{
			World->LineTraceSingleByChannel(
				ConfirmHitResult,
				TraceStart,
				TraceEnd,
				ECC_HitBox
			);
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ConfirmHitResult.GetActor());
			if (BlasterCharacter)
			{
				if (ShotgunResult.BodyShots.Contains(BlasterCharacter))
				{
					ShotgunResult.BodyShots[BlasterCharacter]++;
				}
				else
				{
					ShotgunResult.BodyShots.Emplace(BlasterCharacter, 1);
				}
			}
		}
	}

	// ��Ʈ�ڽ��� ���� ���·� �����ϰ�, �޽� �浹�� �ٽ� Ȱ��ȭ
	for (auto& Frame : CurrentFrames)
	{
		ResetHitBoxes(Frame.Character, Frame);
		EnableCharacterMeshCollision(Frame.Character, ECollisionEnabled::QueryAndPhysics);
	}

	// ���� ���� ����� ��ȯ
	return ShotgunResult;
}


// HitCharacter�� HitCollisionBoxes�� ���� �ڽ��� ��ġ, ȸ��, ũ�� ������ �����մϴ�.
void ULagCompensationComponent::CacheBoxPositions(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage)
{
	// HitCharacter�� ��ȿ���� Ȯ���մϴ�.
	if (HitCharacter == nullptr) return;

	// HitCharacter�� HitCollisionBoxes�� ���� �ݺ��մϴ�.
	for (auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		// HitBoxPair�� ���� ��ȿ���� Ȯ���մϴ�.
		if (HitBoxPair.Value != nullptr)
		{
			// HitBox�� ��ġ, ȸ��, ũ�� ������ �����մϴ�.
			FBoxInformation BoxInfo;
			BoxInfo.Location = HitBoxPair.Value->GetComponentLocation();
			BoxInfo.Rotation = HitBoxPair.Value->GetComponentRotation();
			BoxInfo.BoxExtent = HitBoxPair.Value->GetScaledBoxExtent();

			// OutFramePackage�� HitBoxInfo�� ������ �߰��մϴ�.
			OutFramePackage.HitBoxInfo.Add(HitBoxPair.Key, BoxInfo);
		}
	}
}

// HitCharacter�� HitCollisionBoxes�� ���� ������ ����Ͽ� �ڽ��� ��ġ, ȸ��, ũ�⸦ �̵���ŵ�ϴ�.
void ULagCompensationComponent::MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package)
{
	// HitCharacter�� ��ȿ���� Ȯ���մϴ�.
	if (HitCharacter == nullptr) return;

	// HitCharacter�� HitCollisionBoxes�� ���� �ݺ��մϴ�.
	for (auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		// HitBoxPair�� ���� ��ȿ���� Ȯ���մϴ�.
		if (HitBoxPair.Value != nullptr)
		{
			// Package�� ������ ����Ͽ� HitBox�� ��ġ, ȸ��, ũ�⸦ �̵���ŵ�ϴ�.
			HitBoxPair.Value->SetWorldLocation(Package.HitBoxInfo[HitBoxPair.Key].Location);
			HitBoxPair.Value->SetWorldRotation(Package.HitBoxInfo[HitBoxPair.Key].Rotation);
			HitBoxPair.Value->SetBoxExtent(Package.HitBoxInfo[HitBoxPair.Key].BoxExtent);
		}
	}
}

// HitCharacter�� HitCollisionBoxes�� ���� ������ ����Ͽ� �ڽ��� ��ġ, ȸ��, ũ�⸦ �缳���ϰ� �浹�� ��Ȱ��ȭ�մϴ�.
void ULagCompensationComponent::ResetHitBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package)
{
	// HitCharacter�� ��ȿ���� Ȯ���մϴ�.
	if (HitCharacter == nullptr) return;

	// HitCharacter�� HitCollisionBoxes�� ���� �ݺ��մϴ�.
	for (auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		// HitBoxPair�� ���� ��ȿ���� Ȯ���մϴ�.
		if (HitBoxPair.Value != nullptr)
		{
			// Package�� ������ ����Ͽ� HitBox�� ��ġ, ȸ��, ũ�⸦ �缳���ϰ� �浹�� ��Ȱ��ȭ�մϴ�.
			HitBoxPair.Value->SetWorldLocation(Package.HitBoxInfo[HitBoxPair.Key].Location);
			HitBoxPair.Value->SetWorldRotation(Package.HitBoxInfo[HitBoxPair.Key].Rotation);
			HitBoxPair.Value->SetBoxExtent(Package.HitBoxInfo[HitBoxPair.Key].BoxExtent);
			HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// HitCharacter�� HitCharacter�� Mesh�� �浹�� Ȱ��ȭ �Ǵ� ��Ȱ��ȭ�մϴ�.
void ULagCompensationComponent::EnableCharacterMeshCollision(ABlasterCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled)
{
	// HitCharacter�� HitCharacter�� Mesh�� ��ȿ���� Ȯ���մϴ�.
	if (HitCharacter && HitCharacter->GetMesh())
	{
		// HitCharacter�� Mesh�� �浹�� Ȱ��ȭ �Ǵ� ��Ȱ��ȭ�մϴ�.
		HitCharacter->GetMesh()->SetCollisionEnabled(CollisionEnabled);
	}
}


// FFramePackage�� ����� HitBoxInfo�� ����Ͽ� ����� �ڽ��� �׸��ϴ�. ����� �������� ���˴ϴ�.
void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color)
{
	// FFramePackage�� ����� HitBoxInfo�� �ݺ��Ͽ� ����� �ڽ��� �׸��ϴ�.
	for (auto& BoxInfo : Package.HitBoxInfo)
	{
		DrawDebugBox(
			GetWorld(),
			BoxInfo.Value.Location,
			BoxInfo.Value.BoxExtent,
			FQuat(BoxInfo.Value.Rotation),
			Color,
			false,
			4.f
		);
	}
}

// ��Ʈ�� ĳ���Ϳ� ���� ���� �� ����� �����ϰ�, ��Ʈ ����� Ȯ���մϴ�.
FServerSideRewindResult ULagCompensationComponent::ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	// HitCharacter�� ���� Ȯ���� �������� �����ɴϴ�.
	FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);
	// ConfirmHit �Լ��� ȣ���Ͽ� ��Ʈ Ȯ�� ����� ��ȯ�մϴ�.
	return ConfirmHit(FrameToCheck, HitCharacter, TraceStart, HitLocation);
}

// ����ü�� ���� ���� �� ����� �����ϰ�, ��Ʈ ����� Ȯ���մϴ�.
FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	// HitCharacter�� ���� Ȯ���� �������� �����ɴϴ�.
	FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);
	// ProjectileConfirmHit �Լ��� ȣ���Ͽ� ��Ʈ Ȯ�� ����� ��ȯ�մϴ�.
	return ProjectileConfirmHit(FrameToCheck, HitCharacter, TraceStart, InitialVelocity, HitTime);
}

// ���ǿ� ���� ���� �� ����� �����ϰ�, ��Ʈ ����� Ȯ���մϴ�. HitCharacters�� ���Ե� ��� ĳ���Ϳ� ���� ����� �����մϴ�.
FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunServerSideRewind(const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
	TArray<FFramePackage> FramesToCheck;
	// HitCharacters�� ���� Ȯ���� �������� ��� �����ɴϴ�.
	for (ABlasterCharacter* HitCharacter : HitCharacters)
	{
		FramesToCheck.Add(GetFrameToCheck(HitCharacter, HitTime));
	}

	// ShotgunConfirmHit �Լ��� ȣ���Ͽ� ��Ʈ Ȯ�� ����� ��ȯ�մϴ�.
	return ShotgunConfirmHit(FramesToCheck, TraceStart, HitLocations);
}


// ���ø� �����Ϸ��� ������ ��Ű���� ��ȯ�ϴ� �޼����Դϴ�.
// HitCharacter: ������ ��Ʈ ĳ������ ������
// HitTime: ��Ʈ�� ��Ȯ�� �ð�
FFramePackage ULagCompensationComponent::GetFrameToCheck(ABlasterCharacter* HitCharacter, float HitTime)
{
	// HitCharacter�� ��ȿ���� ���� ��� �� ������ ��Ű���� ��ȯ�մϴ�.
	bool bReturn =
		HitCharacter == nullptr ||
		HitCharacter->GetLagCompensation() == nullptr ||
		HitCharacter->GetLagCompensation()->FrameHistory.GetHead() == nullptr ||
		HitCharacter->GetLagCompensation()->FrameHistory.GetTail() == nullptr;
	if (bReturn) return FFramePackage();

	// ������ ���� Ȯ���� ������ ��Ű���� �����մϴ�.
	FFramePackage FrameToCheck;
	bool bShouldInterpolate = true;

	// HitCharacter�� ������ �����丮�� �����ɴϴ�.
	const TDoubleLinkedList<FFramePackage>& History = HitCharacter->GetLagCompensation()->FrameHistory;
	const float OldestHistoryTime = History.GetTail()->GetValue().Time;
	const float NewestHistoryTime = History.GetHead()->GetValue().Time;

	// ��Ʈ Ÿ���� ���� ������ �����丮 �ð����� ������ ��� (�ʹ� ���� �������� SSR �м��� �Ұ�����)
	if (OldestHistoryTime > HitTime)
	{
		return FFramePackage();
	}

	// HitTime�� ���� ������ �����丮 �ð��� ���� ���
	if (OldestHistoryTime == HitTime)
	{
		FrameToCheck = History.GetTail()->GetValue();
		bShouldInterpolate = false;
	}

	// HitTime�� ���� �ֱ��� �����丮 �ð����� ���ų� ������ ���
	if (NewestHistoryTime <= HitTime)
	{
		FrameToCheck = History.GetHead()->GetValue();
		bShouldInterpolate = false;
	}

	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Younger = History.GetHead();
	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Older = Younger;

	// Older�� HitTime���� ������ �� �̸� �ð����� Ȯ���մϴ�.
	while (Older->GetValue().Time > HitTime)
	{
		// OlderTime < HitTime < YoungerTime�� �ǵ��� �̵��մϴ�.
		if (Older->GetNextNode() == nullptr) break;
		Older = Older->GetNextNode();
		if (Older->GetValue().Time > HitTime)
		{
			Younger = Older;
		}
	}

	// ���� �幰������, ã���� �ϴ� ������ �ð��� ��Ȯ�� ��ġ�ϴ� ���
	if (Older->GetValue().Time == HitTime)
	{
		FrameToCheck = Older->GetValue();
		bShouldInterpolate = false;
	}

	// ������ �ð��� ���̿� �ִ� ��� Younger�� Older ���̿��� ���� ������ �����մϴ�.
	if (bShouldInterpolate)
	{
		FrameToCheck = InterpBetweenFrames(Older->GetValue(), Younger->GetValue(), HitTime);
	}

	// FrameToCheck�� ĳ���͸� �Էµ� HitCharacter�� �����մϴ�.
	FrameToCheck.Character = HitCharacter;
	return FrameToCheck;
}


// ������ ���ھ� ��û�� �����ϴ� �޼����Դϴ�.
// HitCharacter: ���߽�Ų ĳ������ ������
// TraceStart: �ñַ���ĳ��Ʈ�� ���� ��ġ
// HitLocation: �ֵ� ��Ʈ ��ġ
// HitTime: ��Ʈ�� �߻��� �ð�
void ULagCompensationComponent::ServerScoreRequest_Implementation(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	// ���� ������ ���ø� �����ϰ� ��Ʈ ���θ� Ȯ���մϴ�.
	FServerSideRewindResult Confirm = ServerSideRewind(HitCharacter, TraceStart, HitLocation, HitTime);

	// ��Ʈ ���ο� �ٸ� ������ �����Ǹ� ���ظ� �����ϴ�.
	if (Character && HitCharacter && Character->GetEquippedWeapon() && Confirm.bHitConfirmed)
	{
		// ��弦 ���ο� ���� �������� ����մϴ�.
		const float Damage = Confirm.bHeadShot ? Character->GetEquippedWeapon()->GetHeadShotDamage() : Character->GetEquippedWeapon()->GetDamage();

		// ���ظ� �����մϴ�.
		UGameplayStatics::ApplyDamage(
			HitCharacter,
			Damage,
			Character->Controller,
			Character->GetEquippedWeapon(),
			UDamageType::StaticClass()
		);
	}
}

// �߻�ü�� ����Ͽ� ���� ���ھ� ��û�� ó���ϴ� �޼����Դϴ�.
// HitCharacter: �߻�ü�� ���߽�Ų ĳ������ ������
// TraceStart: Ʈ���̽� ���� ��ġ
// InitialVelocity: �߻�ü�� �ʱ� �ӵ�
// HitTime: ��Ʈ�� �߻��� �ð�
void ULagCompensationComponent::ProjectileServerScoreRequest_Implementation(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	// �߻�ü�� ����Ͽ� ���� ������ ���ø� �����ϰ� ��Ʈ ���θ� Ȯ���մϴ�.
	FServerSideRewindResult Confirm = ProjectileServerSideRewind(HitCharacter, TraceStart, InitialVelocity, HitTime);

	// ��Ʈ ���ο� �ٸ� ������ �����Ǹ� ���ظ� �����ϴ�.
	if (&& HitCharacter && Confirm.bHitConfirmed && Character->GetEquippedWeapon())
	{
		// ��弦 ���ο� ���� �������� ����մϴ�.
		const float Damage = Confirm.bHeadShot ? Character->GetEquippedWeapon()->GetHeadShotDamage() : Character->GetEquippedWeapon()->GetDamage();

		// ���ظ� �����մϴ�.
		UGameplayStatics::ApplyDamage(
			HitCharacter,
			Damage,
			Character->Controller,
			Character->GetEquippedWeapon(),
			UDamageType::StaticClass()
		);
	}
}


// ���� ���� ���ھ� ��û�� ó���ϴ� �޼����Դϴ�.
// HitCharacters: ���߽�Ų ĳ������ ������ �迭
// TraceStart: Ʈ���̽� ���� ��ġ
// HitLocations: �ֵ� ��Ʈ ��ġ�� �迭
// HitTime: ��Ʈ�� �߻��� �ð�
void ULagCompensationComponent::ShotgunServerScoreRequest_Implementation(const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
	// ������ ����Ͽ� ���� ������ ���ø� �����ϰ� ��Ʈ ���θ� Ȯ���մϴ�.
	FShotgunServerSideRewindResult Confirm = ShotgunServerSideRewind(HitCharacters, TraceStart, HitLocations, HitTime);

	// ������ ���� ĳ���Ϳ� ���ؼ�
	for (auto& HitCharacter : HitCharacters)
	{
		// ĳ����, ����, ���� ���Ⱑ ��ȿ���� Ȯ���մϴ�.
		if (HitCharacter == nullptr || HitCharacter->GetEquippedWeapon() == nullptr || Character == nullptr) continue;

		float TotalDamage = 0.f;

		// ��弦�� �߻��� ��� �������� ����մϴ�.
		if (Confirm.HeadShots.Contains(HitCharacter))
		{
			float HeadShotDamage = Confirm.HeadShots[HitCharacter] * HitCharacter->GetEquippedWeapon()->GetHeadShotDamage();
			TotalDamage += HeadShotDamage;
		}

		// ����ź�� �߻��� ��� �������� ����մϴ�.
		if (Confirm.BodyShots.Contains(HitCharacter))
		{
			float BodyShotDamage = Confirm.BodyShots[HitCharacter] * HitCharacter->GetEquippedWeapon()->GetDamage();
			TotalDamage += BodyShotDamage;
		}

		// ���� ���ظ� �����մϴ�.
		UGameplayStatics::ApplyDamage(
			HitCharacter,
			TotalDamage,
			Character->Controller,
			HitCharacter->GetEquippedWeapon(),
			UDamageType::StaticClass()
		);
	}
}

// ������Ʈ�� ƽ�� ������ ȣ��Ǵ� �޼����Դϴ�.
// DeltaTime: ���� ������ ���� ���� �ð�
// TickType: ƽ�ϴ� ����
// ThisTickFunction: �� ƽ �Լ��� ������
void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// �θ� Ŭ������ TickComponent�� ȣ���մϴ�.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �� ������ ��Ű���� �����մϴ�.
	SaveFramePackage();
}


// �����Ӻ� ��Ű���� �����ϴ� �޼����Դϴ�.
void ULagCompensationComponent::SaveFramePackage()
{
	if (Character == nullptr || !Character->HasAuthority()) return;

	// ������ ����� �� ��� ���� ��Ű���� ����մϴ�.
	if (FrameHistory.Num() <= 1)
	{
		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);
	}
	else
	{
		// �ִ� ��� �ð��� �ʰ��� ��� ������ ����� �����մϴ�.
		float HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		while (HistoryLength > MaxRecordTime)
		{
			FrameHistory.RemoveNode(FrameHistory.GetTail());
			HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		}

		// ���� ��Ű���� ������ ��Ͽ� �߰��մϴ�.
		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);

		// ����� �������� ��Ű���� ǥ���մϴ�.
		// ShowFramePackage(ThisFrame, FColor::Red);
	}
}

// ��Ű�� ������ �����ϴ� �޼����Դϴ�.
// Package: ������ ��Ű�� ����
void ULagCompensationComponent::SaveFramePackage(FFramePackage& Package)
{
	// ĳ���͸� ������ �� ��Ű�� ������ ä��ϴ�.
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : Character;
	if (Character)
	{
		Package.Time = GetWorld()->GetTimeSeconds();
		Package.Character = Character;

		// �� ��Ʈ �ڽ� ������ ��Ű���� �߰��մϴ�.
		for (auto& BoxPair : Character->HitCollisionBoxes)
		{
			FBoxInformation BoxInformation;
			BoxInformation.Location = BoxPair.Value->GetComponentLocation();
			BoxInformation.Rotation = BoxPair.Value->GetComponentRotation();
			BoxInformation.BoxExtent = BoxPair.Value->GetScaledBoxExtent();
			Package.HitBoxInfo.Add(BoxPair.Key, BoxInformation);
		}
	}
}

