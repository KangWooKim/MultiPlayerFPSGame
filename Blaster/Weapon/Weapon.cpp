// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Casing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
// AWeapon Ŭ���� ������
AWeapon::AWeapon()
{
	// ������ ƽ ���� ���� ����. �� ���ʹ� ƽ���� �ʽ��ϴ�.
	PrimaryActorTick.bCanEverTick = false;

	// ���� ���� ���� ����. �� ���ʹ� ���� �����մϴ�.
	bReplicates = true;

	// ������ ���� ���� ����. �� ������ �������� �����˴ϴ�.
	SetReplicateMovement(true);

	// ���� �޽� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// ���� �޽��� �浹 ���� ����
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ŀ���� ���� Ȱ��ȭ �� ����
	EnableCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponMesh->MarkRenderStateDirty();

	// ���� �� ������Ʈ ���� �� ����
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �Ⱦ� ���� ������Ʈ ���� �� ����
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

// Ŀ���� ���� Ȱ��ȭ �޼���
// bEnable: Ŀ���� ������ Ȱ��ȭ���� ���θ� �����ϴ� �Ҹ��� ���Դϴ�. �� ���� true�̸� Ŀ���� ������ Ȱ��ȭ�ǰ�, false�̸� ��Ȱ��ȭ�˴ϴ�.
void AWeapon::EnableCustomDepth(bool bEnable)
{
	if (WeaponMesh)
	{
		// ���� �޽��� Ŀ���� ���� ����
		WeaponMesh->SetRenderCustomDepth(bEnable);
	}
}

// ������ ���۵Ǿ��� ���� ������ �����ϴ� �޼���
void AWeapon::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay ȣ��
	Super::BeginPlay();

	// ���� ���� �浹 ����
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);

	if (PickupWidget)
	{
		// �Ⱦ� ���� ��Ȱ��ȭ
		PickupWidget->SetVisibility(false);
	}
}


// ���⿡ ���� ƽ �޼���
void AWeapon::Tick(float DeltaTime)
{
	// �θ� Ŭ������ ƽ �޼��� ȣ��
	Super::Tick(DeltaTime);
}

// �����Ǵ� �Ӽ����� �����ϴ� �޼���
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// �θ� Ŭ������ ���� �Ӽ� ���� �޼��� ȣ��
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ���� ���¿� ���� �� �ǰ��� ��� ���θ� �����Ǵ� �Ӽ����� ����
	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME_CONDITION(AWeapon, bUseServerSideRewind, COND_OwnerOnly);
}

// �� ������Ʈ���� �������� ���۵� �� ȣ��Ǵ� �޼���
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);

	// ������ ĳ���Ͱ� �������� ���۵� ���
	if (BlasterCharacter)
	{
		// ���� ���Ⱑ �÷��� �����̰�, �������� ĳ���Ͱ� ���� ���� ��� ����
		if (WeaponType == EWeaponType::EWT_Flag && BlasterCharacter->GetTeam() == Team) return;

		// �������� ĳ���Ͱ� �̹� �÷��׸� �����ϰ� �ִ� ��� ����
		if (BlasterCharacter->IsHoldingTheFlag()) return;

		// �������� ĳ���Ϳ��� ���� �������� ���⸦ ����
		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

// �� ������Ʈ���� �������� ����� �� ȣ��Ǵ� �޼���
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// ���� ���Ⱑ �÷��� �����̰�, �������� ĳ���Ͱ� ���� ���� ��� ����
		if (WeaponType == EWeaponType::EWT_Flag && BlasterCharacter->GetTeam() == Team) return;

		// �������� ĳ���Ͱ� �̹� �÷��׸� �����ϰ� �ִ� ��� ����
		if (BlasterCharacter->IsHoldingTheFlag()) return;

		// �������� ĳ���Ϳ��� �������� ���⸦ null�� ����
		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}
}

// HUD�� ǥ�õǴ� ź�� ������ �����ϴ� �޼���
void AWeapon::SetHUDAmmo()
{
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if (BlasterOwnerController)
		{
			// ��Ʈ�ѷ��� ���� HUD�� ���� ź�� ������ ����
			BlasterOwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

// ���⸦ �߻��Ͽ� ź���� �Ҹ��ϴ� �޼���
void AWeapon::SpendRound()
{
	// ���� ź���� �ϳ� ���ҽ�Ű��, 0�� źâ �뷮 ���̷� Ŭ����
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);

	// HUD�� ź�� ���� ������Ʈ
	SetHUDAmmo();

	// ���� ������ �ִ� ��� Ŭ���̾�Ʈ�� ź�� ������ ������Ʈ
	if (HasAuthority())
	{
		ClientUpdateAmmo(Ammo);
	}
	else
	{
		++Sequence;
	}
}


// Ŭ���̾�Ʈ ������ �����κ��� ���� ź�� ������ ������Ʈ�ϴ� �޼���
void AWeapon::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{
	// ���� ������ �ִ� ��� ����
	if (HasAuthority()) return;

	// �����κ��� ���� ź�� ������ ����
	Ammo = ServerAmmo;

	// ������ ����
	--Sequence;

	// ź���� ��������ŭ ���ҽ�Ű��, HUD�� ź�� ������ ������Ʈ
	Ammo -= Sequence;
	SetHUDAmmo();
}

// ź���� �߰��ϴ� �޼���
void AWeapon::AddAmmo(int32 AmmoToAdd)
{
	// ź���� �߰��ϰ�, 0�� źâ �뷮 ���̷� Ŭ����
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);

	// HUD�� ź�� ������ ������Ʈ�ϰ�, Ŭ���̾�Ʈ�� ź�� �߰� ������ ����
	SetHUDAmmo();
	ClientAddAmmo(AmmoToAdd);
}

// Ŭ���̾�Ʈ ������ ź���� �߰��ϴ� �޼���
void AWeapon::ClientAddAmmo_Implementation(int32 AmmoToAdd)
{
	// ���� ������ �ִ� ��� ����
	if (HasAuthority()) return;

	// ź���� �߰��ϰ�, 0�� źâ �뷮 ���̷� Ŭ����
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);

	// ������ ���� ĳ���͸� ��������, �ش� ĳ������ ���� ���¸� Ȯ���� ��, źâ�� ���� �� ���, ���� ����� �̵�
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter && BlasterOwnerCharacter->GetCombat() && IsFull())
	{
		BlasterOwnerCharacter->GetCombat()->JumpToShotgunEnd();
	}

	// HUD�� ź�� ������ ������Ʈ
	SetHUDAmmo();
}

// ���� �����ڿ� ���� ������ �̷������ �� ȣ��Ǵ� �޼���
void AWeapon::OnRep_Owner()
{
	// �θ� Ŭ������ ���� ������ ���� �޼��� ȣ��
	Super::OnRep_Owner();

	// �����ڰ� ���� ���, ������ ���� ĳ���Ϳ� ��Ʈ�ѷ��� null�� ����
	if (Owner == nullptr)
	{
		BlasterOwnerCharacter = nullptr;
		BlasterOwnerController = nullptr;
	}
	else
	{
		// �����ڰ� �����ϴ� ���, ������ ���� ĳ���͸� �����ϰ�, �ش� ĳ���Ͱ� ���⸦ �����ϰ� �ִ��� Ȯ���� ��, HUD�� ź�� ������ ������Ʈ
		BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(Owner) : BlasterOwnerCharacter;
		if (BlasterOwnerCharacter && BlasterOwnerCharacter->GetEquippedWeapon() && BlasterOwnerCharacter->GetEquippedWeapon() == this)
		{
			SetHUDAmmo();
		}
	}
}

// ���� ���¸� �����ϴ� �޼���
void AWeapon::SetWeaponState(EWeaponState State)
{
	// ���� ���¸� �����ϰ�, ���� ���� ���� �̺�Ʈ�� ȣ��
	WeaponState = State;
	OnWeaponStateSet();
}


// ������ ���¿� ���� ������ ó���� �����ϴ� �޼���
void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
		// ���Ⱑ ������ ������ ���� ó��
	case EWeaponState::EWS_Equipped:
		OnEquipped();
		break;
		// ���Ⱑ ���� ���� ������ ������ ���� ó��
	case EWeaponType::EWT_EquippedSecondary:
		OnEquippedSecondary();
		break;
		// ���Ⱑ ����� ������ ���� ó��
	case EWeaponState::EWS_Dropped:
		OnDropped();
		break;
	}
}

// ���� �ʹ� ���� ���, ���� �� �����ε带 ������� �ʵ��� �����ϴ� �޼���
void AWeapon::OnPingTooHigh(bool bPingTooHigh) // bPingTooHigh: ���� �ʹ� �������� ��Ÿ���� ����
{
	bUseServerSideRewind = !bPingTooHigh;
}

// ���� ���¿� ���� ������ �̷������ �� ȣ��Ǵ� �޼���
void AWeapon::OnRep_WeaponState()
{
	OnWeaponStateSet();
}

// ���Ⱑ �����Ǿ��� �� ȣ��Ǵ� �޼���
void AWeapon::OnEquipped()
{
	// �Ⱦ� ���� ����
	ShowPickupWidget(false);

	// �浹�� ��Ȱ��ȭ�ϰ� ���� �ùķ��̼ǰ� �߷��� ��Ȱ��ȭ
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���Ⱑ ����ӽŰ��� ���, �浹�� �߷��� Ȱ��ȭ�ϰ� ��� ä�ο� ���� �浹 ������ ����
	if (WeaponType == EWeaponType::EWT_SubmachineGun)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	// ����� ���� ���̸� ��Ȱ��ȭ
	EnableCustomDepth(false);

	// ������ ���� ĳ���͸� ��������, ���� �� �����ε带 ����ϴ� ���,
	// ������ ���� ��Ʈ�ѷ��� �����ͼ� ���� �ʹ� ���� ��쿡 ���� ��������Ʈ�� �߰�
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if (BlasterOwnerController && HasAuthority() && !BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.AddDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}


// ���Ⱑ ����Ǿ��� �� ȣ��Ǵ� �޼���
void AWeapon::OnDropped()
{
	// ���� �� ��ü�� ������ ������ �ִٸ�, AreaSphere�� �浹�� ������ �����ϵ��� ����
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	// WeaponMesh�� ���� �ùķ��̼ǰ� �߷��� Ȱ��ȭ, �浹�� ������ ������ ����
	// ��� ä���� �浹 ������ �����ϰ�, Pawn�� Camera ä���� �浹 ������ �����ϵ��� ����
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);

	// ����� ���� ���� ���ٽ� ���� �Ķ������� �����ϰ� ���� ���¸� ���������� ǥ��
	// ����� ���� ���̸� Ȱ��ȭ
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);

	// ������ ���� ĳ���͸� ��������,
	// ������ ���� ��Ʈ�ѷ��� �����ͼ� ���� �ʹ� ���� ��쿡 ���� ��������Ʈ�� ����
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if (BlasterOwnerController && HasAuthority() && BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

// ���Ⱑ ���� ���� �����Ǿ��� �� ȣ��Ǵ� �޼���
void AWeapon::OnEquippedSecondary()
{
	// �Ⱦ� ���� ����
	// �浹�� ��Ȱ��ȭ�ϰ� ���� �ùķ��̼ǰ� �߷��� ��Ȱ��ȭ
	// ���Ⱑ ����ӽŰ��� ���, �浹�� �߷��� Ȱ��ȭ�ϰ� ��� ä�ο� ���� �浹 ������ ����
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (WeaponType == EWeaponType::EWT_SubmachineGun)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	// ���� �޽ð� �ִٸ�, ����� ���� ���� ���ٽ� ���� ź������ �����ϰ� ���� ���¸� ���������� ǥ��
	if (WeaponMesh)
	{
		WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
		WeaponMesh->MarkRenderStateDirty();
	}

	// ������ ���� ĳ���͸� ��������,
	// ������ ���� ��Ʈ�ѷ��� �����ͼ� ���� �ʹ� ���� ��쿡 ���� ��������Ʈ�� ����
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if (BlasterOwnerController && HasAuthority() && BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}


// �Ⱦ� ������ ���ü��� �����ϴ� �޼���
void AWeapon::ShowPickupWidget(bool bShowWidget) // bShowWidget: ������ ǥ������ ���θ� �����ϴ� �Ҹ���
{
	// PickupWidget�� �ִٸ� ���ü��� ����
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

// ���⸦ �߻��ϴ� �޼���
void AWeapon::Fire(const FVector& HitTarget) // HitTarget: Ÿ�� ����� ��ġ
{
	// �߻� �ִϸ��̼��� �ִٸ� �ִϸ��̼� ���
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	// ź�� Ŭ������ �ִٸ�, ź�Ǹ� �����ϴ� ���Ͽ��� ź�Ǹ� ����
	if (CasingClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ACasing>(
					CasingClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
					);
			}
		}
	}
	// �Ѿ��� ���
	SpendRound();
}

// ���Ⱑ ����Ǿ��� �� ȣ��Ǵ� �޼���
void AWeapon::Dropped()
{
	// ���� ���¸� '�����'���� ����
	SetWeaponState(EWeaponState::EWS_Dropped);

	// ���� �޽ø� ���� ��ȯ ��Ģ�� ���� ������Ʈ�κ��� �и�
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);

	// ������ �����ڿ� ĳ����, ��Ʈ�ѷ��� nullptr�� ����
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}

// ������ źâ�� ����ִ��� Ȯ���ϴ� �޼���
bool AWeapon::IsEmpty()
{
	return Ammo <= 0; // źâ�� �����ִ� �Ѿ��� ������ 0���ϸ� true ��ȯ
}

// ������ źâ�� ���� �� �ִ��� Ȯ���ϴ� �޼���
bool AWeapon::IsFull()
{
	return Ammo == MagCapacity; // źâ�� �����ִ� �Ѿ��� ������ źâ �뷮�� ������ true ��ȯ
}

// ��ĳ��(������ ����)�� ����� Ʈ���̽� ���� ����Ʈ�� ����ϴ� �޼���
FVector AWeapon::TraceEndWithScatter(const FVector& HitTarget) // HitTarget: Ÿ�� ����� ��ġ
{
	// �����÷��� ���� ���� ȹ��
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	// ������ ���ٸ� �� ���� ��ȯ
	if (MuzzleFlashSocket == nullptr) return FVector();

	// ������ ��ȯ(��ġ, ����, ũ��) ���� ȹ��
	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

	// Ʈ���̽� ���� ��ġ�� ���� ��ġ�� ����
	const FVector TraceStart = SocketTransform.GetLocation();

	// Ÿ�� ����� ���ϴ� ���� ���͸� ����ȭ
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();

	// ���Ǿ�(��ü) �߽� ��ġ ���
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	// ������ ���͸� �����Ͽ� ���Ǿ� �ݰ� ������ ��ġ ���
	const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);

	// ���� ��ġ ���
	const FVector EndLoc = SphereCenter + RandVec;
	const FVector ToEndLoc = EndLoc - TraceStart;

	/*
	// ����� �������� ���Ǿ�� ������ �׸�
	DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, true);
	DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, true);
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()),
		FColor::Cyan,
		true);*/

		// ���� Ʈ���̽� ���� ����Ʈ�� ��ȯ
	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
}
