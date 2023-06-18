// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location; // ��Ʈ �ڽ��� ��ġ�� ��Ÿ���ϴ�.

	UPROPERTY()
	FRotator Rotation; // ��Ʈ �ڽ��� ȸ������ ��Ÿ���ϴ�.

	UPROPERTY()
	FVector BoxExtent; // ��Ʈ �ڽ��� ũ�⸦ ��Ÿ���ϴ�.
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time; // ������ ��Ű���� �ð��� ��Ÿ���ϴ�.

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo; // ��Ʈ �ڽ� ������ �����մϴ�.

	UPROPERTY()
	ABlasterCharacter* Character; // ������ ��Ű���� ���õ� ĳ���͸� �����մϴ�.
};

USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHitConfirmed; // Ÿ�� Ȯ�� ���θ� ��Ÿ���ϴ�.

	UPROPERTY()
	bool bHeadShot; // ��弦 ���θ� ��Ÿ���ϴ�.
};

USTRUCT(BlueprintType)
struct FShotgunServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ABlasterCharacter*, uint32> HeadShots; // �ѱ� ��� �� ��弦 Ƚ���� �����մϴ�.

	UPROPERTY()
	TMap<ABlasterCharacter*, uint32> BodyShots; // �ѱ� ��� �� ���� �� Ƚ���� �����մϴ�.

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ������
	ULagCompensationComponent();

	// ABlasterCharacter Ŭ������ friend Ŭ����
	friend class ABlasterCharacter;

	// ������Ʈ�� Tick �Լ�
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������ ��Ű���� �����ִ� �Լ�
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color);

	/**
	* Hitscan
	*/

	// ���� ���̵� �����ε� �Լ�
	// ����:
	//   - HitCharacter: Ÿ���� ���� ABlasterCharacter ��ü
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - HitLocation: Ÿ�� ��ġ (FVector_NetQuantize)
	//   - HitTime: Ÿ�� �ð�
	FServerSideRewindResult ServerSideRewind(
		class ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime);

	/**
	* Projectile
	*/

	// ������Ÿ�� ���� ���̵� �����ε� �Լ�
	// ����:
	//   - HitCharacter: Ÿ���� ���� ABlasterCharacter ��ü
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - InitialVelocity: �ʱ� �ӵ� (FVector_NetQuantize100)
	//   - HitTime: Ÿ�� �ð�
	FServerSideRewindResult ProjectileServerSideRewind(
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

	/**
	* Shotgun
	*/

	// ���� ���� ���̵� �����ε� �Լ�
	// ����:
	//   - HitCharacters: Ÿ���� ���� ABlasterCharacter ��ü���� �迭
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - HitLocations: Ÿ�� ��ġ���� �迭 (FVector_NetQuantize)
	//   - HitTime: Ÿ�� �ð�
	FShotgunServerSideRewindResult ShotgunServerSideRewind(
		const TArray<ABlasterCharacter*>& HitCharacters,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations,
		float HitTime);

	// �������� ���� ��û�ϴ� �Լ�
	// ����:
	//   - HitCharacter: Ÿ���� ���� ABlasterCharacter ��ü
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - HitLocation: Ÿ�� ��ġ (FVector_NetQuantize)
	//   - HitTime: Ÿ�� �ð�
	UFUNCTION(Server, Reliable)
		void ServerScoreRequest(
			ABlasterCharacter* HitCharacter,
			const FVector_NetQuantize& TraceStart,
			const FVector_NetQuantize& HitLocation,
			float HitTime
		);

	// ������Ÿ�� �������� ���� ��û�ϴ� �Լ�
	// ����:
	//   - HitCharacter: Ÿ���� ���� ABlasterCharacter ��ü
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - InitialVelocity: �ʱ� �ӵ� (FVector_NetQuantize100)
	//   - HitTime: Ÿ�� �ð�
	UFUNCTION(Server, Reliable)
		void ProjectileServerScoreRequest(
			ABlasterCharacter* HitCharacter,
			const FVector_NetQuantize& TraceStart,
			const FVector_NetQuantize100& InitialVelocity,
			float HitTime
		);

	// ���� �������� ���� ��û�ϴ� �Լ�
	// ����:
	//   - HitCharacters: Ÿ���� ���� ABlasterCharacter ��ü���� �迭
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - HitLocations: Ÿ�� ��ġ���� �迭 (FVector_NetQuantize)
	//   - HitTime: Ÿ�� �ð�
	UFUNCTION(Server, Reliable)
		void ShotgunServerScoreRequest(
			const TArray<ABlasterCharacter*>& HitCharacters,
			const FVector_NetQuantize& TraceStart,
			const TArray<FVector_NetQuantize>& HitLocations,
			float HitTime
		);
protected:
	// ���� ���� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;

	// ������ ��Ű���� �����ϴ� �Լ�
	// ����:
	//   - Package: ������ ������ ��Ű�� (FFramePackage)
	void SaveFramePackage(FFramePackage& Package);

	// �� ������ ���̸� �����Ͽ� Ÿ�� �ð��� �ش��ϴ� ������ ��Ű���� ��ȯ�ϴ� �Լ�
	// ����:
	//   - OlderFrame: ���� ������ ��Ű�� (FFramePackage)
	//   - YoungerFrame: ���� ������ ��Ű�� (FFramePackage)
	//   - HitTime: Ÿ�� �ð�
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

	// HitCharacter�� �ڽ� ��ġ�� ĳ���ϴ� �Լ�
	// ����:
	//   - HitCharacter: �ڽ� ��ġ�� ĳ���� ABlasterCharacter ��ü
	//   - OutFramePackage: �ڽ� ��ġ�� ������ ������ ��Ű�� (FFramePackage)
	void CacheBoxPositions(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage);

	// HitCharacter�� �ڽ����� �̵���Ű�� �Լ�
	// ����:
	//   - HitCharacter: �̵��� �ڽ����� ������ �ִ� ABlasterCharacter ��ü
	//   - Package: �̵��� ����� ������ ��Ű�� (FFramePackage)
	void MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	// HitCharacter�� �ڽ����� �ʱ� ��ġ�� �缳���ϴ� �Լ�
	// ����:
	//   - HitCharacter: �ʱ� ��ġ�� �缳���� �ڽ����� ������ �ִ� ABlasterCharacter ��ü
	//   - Package: �ʱ� ��ġ�� ����� ������ ��Ű�� (FFramePackage)
	void ResetHitBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	// HitCharacter�� �޽� �浹�� Ȱ��ȭ�ϴ� �Լ�
	// ����:
	//   - HitCharacter: �޽� �浹�� Ȱ��ȭ�� ABlasterCharacter ��ü
	//   - CollisionEnabled: Ȱ��ȭ�� �浹 ���� (ECollisionEnabled::Type)
	void EnableCharacterMeshCollision(ABlasterCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled);

	// ������ ��Ű���� �����ϴ� �Լ�
	void SaveFramePackage();

	// HitCharacter�� Ư�� �ð��� �ش��ϴ� ������ ��Ű���� ��ȯ�ϴ� �Լ�
	// ����:
	//   - HitCharacter: ������ ��Ű���� Ȯ���� ABlasterCharacter ��ü
	//   - HitTime: Ÿ�� �ð�
	FFramePackage GetFrameToCheck(ABlasterCharacter* HitCharacter, float HitTime);

	/**
	* Hitscan
	*/

	// Ÿ���� Ȯ���ϴ� �Լ�
	// ����:
	//   - Package: ������ ��Ű�� (FFramePackage)
	//   - HitCharacter: Ÿ���� ���� ABlasterCharacter ��ü
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - HitLocation: Ÿ�� ��ġ (FVector_NetQuantize)
	FServerSideRewindResult ConfirmHit(
		const FFramePackage& Package,
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation);

	/**
	* Projectile
	*/

	// ������Ÿ�� Ÿ���� Ȯ���ϴ� �Լ�
	// ����:
	//   - Package: ������ ��Ű�� (FFramePackage)
	//   - HitCharacter: Ÿ���� ���� ABlasterCharacter ��ü
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - InitialVelocity: �ʱ� �ӵ� (FVector_NetQuantize100)
	//   - HitTime: Ÿ�� �ð�
	FServerSideRewindResult ProjectileConfirmHit(
		const FFramePackage& Package,
		ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

	/**
	* Shotgun
	*/

	// ���� Ÿ���� Ȯ���ϴ� �Լ�
	// ����:
	//   - FramePackages: ������ ��Ű������ �迭 (TArray<FFramePackage>)
	//   - TraceStart: ����ĳ��Ʈ�� ���� ��ġ (FVector_NetQuantize)
	//   - HitLocations: Ÿ�� ��ġ���� �迭 (TArray<FVector_NetQuantize>)
	FShotgunServerSideRewindResult ShotgunConfirmHit(
		const TArray<FFramePackage>& FramePackages,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations
	);
private:

	// ĳ���� ��ü
	UPROPERTY()
	ABlasterCharacter* Character;

	// �÷��̾� ��Ʈ�ѷ� ��ü
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	// ������ ����� ���� ���� ���� ����Ʈ
	TDoubleLinkedList<FFramePackage> FrameHistory;

	// �ִ� ��� �ð�
	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;


public:	
	
		
};
