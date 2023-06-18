// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// �⺻ ������
	UCombatComponent();

	friend class ABlasterCharacter;

	// ������Ʈ ƽ �Լ�
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ���ø����̼� �Ӽ� ���� �Լ�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���� ���� �Լ�
	// WeaponToEquip : ������ ����
	void EquipWeapon(class AWeapon* WeaponToEquip);

	// ���� ��ü �Լ�
	void SwapWeapons();

	// ������ �Լ�
	void Reload();

	// ������ �Ϸ� �Լ�
	UFUNCTION(BlueprintCallable)
		void FinishReloading();

	// ���� ��ü �Ϸ� �Լ�
	UFUNCTION(BlueprintCallable)
		void FinishSwap();

	// ���� ��ü �������� ���� ÷�� �Ϸ� �Լ�
	UFUNCTION(BlueprintCallable)
		void FinishSwapAttachWeapons();

	// �߻� ��ư�� ���� �������� �����ϴ� �Լ�
	// bPressed : ��ư�� ���� �������� ��Ÿ��
	void FireButtonPressed(bool bPressed);

	// ���� źâ ������ �Լ�
	UFUNCTION(BlueprintCallable)
		void ShotgunShellReload();

	// ���� ������ ���� �Լ�
	void JumpToShotgunEnd();

	// ����ź ������ �Ϸ� �Լ�
	UFUNCTION(BlueprintCallable)
		void ThrowGrenadeFinished();

	// ����ź �߻� �Լ�
	UFUNCTION(BlueprintCallable)
		void LaunchGrenade();

	// �������� ����ź �߻� �Լ�
	// Target : ����ź �߻� ��ǥ
	UFUNCTION(Server, Reliable)
		void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	// ź�� ȹ�� �Լ�
	// WeaponType : ���� ����
	// AmmoAmount : ȹ���� ź�� ��
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	// ���ÿ��� ������ ������ ��Ÿ���� ����
	bool bLocallyReloading = false;

protected:
	// ���� �÷��̰� ���۵� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;

	// ���� ���� ���� �Լ�
	// bIsAiming : ���� �������� ��Ÿ��
	void SetAiming(bool bIsAiming);

	// �������� ���� ���� ���� �Լ�
	// bIsAiming : ���� �������� ��Ÿ��
	UFUNCTION(Server, Reliable)
		void ServerSetAiming(bool bIsAiming);

	// �� ���� ������ ���ø����̼� �Լ�
	UFUNCTION()
		void OnRep_EquippedWeapon();

	// ���� ������ ���ø����̼� �Լ�
	UFUNCTION()
		void OnRep_SecondaryWeapon();

	// �߻� �Լ�
	void Fire();

	// �߻�ü ���� �߻� �Լ�
	void FireProjectileWeapon();

	// ��Ʈ��ĵ ���� �߻� �Լ�
	void FireHitScanWeapon();

	// ���� �߻� �Լ�
	void FireShotgun();

	// ���ÿ����� �߻� ó�� �Լ�
	// TraceHitTarget : �߻� ��� Ÿ��
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);

	// ���ÿ����� ���� �߻� ó�� �Լ�
	// TraceHitTargets : ���� �߻� ��� Ÿ�ٵ�
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	// ���������� �߻� ó�� �Լ�
	// TraceHitTarget : �߻� ��� Ÿ��
	// FireDelay : �߻� ������
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	// ��Ƽĳ��Ʈ �߻� �Լ�
	// TraceHitTarget : �߻� ��� Ÿ��
	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	// ���������� ���� �߻� ó�� �Լ�
	// TraceHitTargets : ���� �߻� ��� Ÿ�ٵ�
	// FireDelay : �߻� ������
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);

	// ��Ƽĳ��Ʈ ���� �߻� �Լ�
	// TraceHitTargets : ���� �߻� ��� Ÿ�ٵ�
	UFUNCTION(NetMulticast, Reliable)
		void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	// ������ �Ʒ��� ��Ʈ ������ �������� �Լ�
	// TraceHitResult : ��Ʈ ���
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	// HUD ũ�ν���� ���� �Լ�
	// DeltaTime : ��Ÿ �ð�
	void SetHUDCrosshairs(float DeltaTime);

	// ���������� ������ �Լ�
	UFUNCTION(Server, Reliable)
		void ServerReload();

	// ������ ó�� �Լ�
	void HandleReload();

	// �������� ź�෮�� ��ȯ�ϴ� �Լ�
	int32 AmountToReload();

	// ����ź ������ �Լ�
	void ThrowGrenade();

	// �������� ����ź ������ �Լ�
	UFUNCTION(Server, Reliable)
		void ServerThrowGrenade();

	// ����ź Ŭ����
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AProjectile> GrenadeClass;

	// �������� ���� ������ �Լ�
	void DropEquippedWeapon();

	// �����տ� ���� ÷�� �Լ�
	// ActorToAttach : ÷���� ����
	void AttachActorToRightHand(AActor* ActorToAttach);

	// �޼տ� ���� ÷�� �Լ�
	// ActorToAttach : ÷���� ����
	void AttachActorToLeftHand(AActor* ActorToAttach);

	// �޼տ� ��� ÷�� �Լ�
	// Flag : ÷���� ���
	void AttachFlagToLeftHand(AWeapon* Flag);

	// �賶�� ���� ÷�� �Լ�
	// ActorToAttach : ÷���� ����
	void AttachActorToBackpack(AActor* ActorToAttach);

	// ź�� ������Ʈ �Լ�
	void UpdateCarriedAmmo();

	// ���� ���� �Ҹ� ��� �Լ�
	// WeaponToEquip : ������ ����
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);

	// �� ���� ������ �Լ�
	void ReloadEmptyWeapon();

	// ����ź �����ֱ� �Լ�
	// bShowGrenade : ����ź�� �������� ����
	void ShowAttachedGrenade(bool bShowGrenade);

	// �� ���� ���� �Լ�
	// WeaponToEquip : ������ ����
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);

	// ���� ���� ���� �Լ�
	// WeaponToEquip : ������ ����
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

private:
	// ĳ���� Ŭ����
	UPROPERTY()
		class ABlasterCharacter* Character;

	// �÷��̾� ��Ʈ�ѷ� Ŭ����
	UPROPERTY()
		class ABlasterPlayerController* Controller;

	// HUD Ŭ����
	UPROPERTY()
		class ABlasterHUD* HUD;

	// ������ ����
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
		AWeapon* EquippedWeapon;

	// ���� ����
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
		AWeapon* SecondaryWeapon;

	// ������ ����
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
		bool bAiming = false;

	// ���� ��ư ���� ����
	bool bAimButtonPressed = false;

	// ���� ������ ���ø����̼� �Լ�
	UFUNCTION()
		void OnRep_Aiming();

	// �⺻ �ȱ� �ӵ�
	UPROPERTY(EditAnywhere)
		float BaseWalkSpeed;

	// ���ؽ� �ȱ� �ӵ�
	UPROPERTY(EditAnywhere)
		float AimWalkSpeed;

	// �߻� ��ư ���� ����
	bool bFireButtonPressed;

	/**
	* HUD�� ũ�ν����
	*/

	// ũ�ν������ �ӵ� ���
	float CrosshairVelocityFactor;

	// ���߿� �ִ� ������ ũ�ν���� ���
	float CrosshairInAirFactor;

	// �����ϴ� ������ ũ�ν���� ���
	float CrosshairAimFactor;

	// �߻��ϴ� ������ ũ�ν���� ���
	float CrosshairShootingFactor;

	// ��Ʈ Ÿ�� ��ġ
	FVector HitTarget;

	// HUD ��Ű��
	FHUDPackage HUDPackage;

	/**
	* ���� �� �þ߰�(FOV)
	*/

	// �������� �ʴ� ������ �þ߰�; BeginPlay���� ī�޶��� �⺻ FOV�� ������
	float DefaultFOV;

	// ���� ���� �þ߰�
	UPROPERTY(EditAnywhere, Category = Combat)
		float ZoomedFOV = 30.f;

	// ���� �þ߰�
	float CurrentFOV;

	// �þ߰� ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = Combat)
		float ZoomInterpSpeed = 20.f;

	// �þ߰� ���� �Լ�
	// DeltaTime : ��Ÿ �ð�
	void InterpFOV(float DeltaTime);

	/**
	* �ڵ� �߻�
	*/

	// �߻� Ÿ�̸�
	FTimerHandle FireTimer;

	// �߻� ���� ����
	bool bCanFire = true;

	// �߻� Ÿ�̸� ���� �Լ�
	void StartFireTimer();

	// �߻� Ÿ�̸� ���� �Լ�
	void FireTimerFinished();

	// �߻� ���� ���� ��ȯ �Լ�
	bool CanFire();

	// ���� ������ ���⿡ ���� ���� ź�෮
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
		int32 CarriedAmmo;

	// ���� ź�෮�� ���ø����̼� �Լ�
	UFUNCTION()
		void OnRep_CarriedAmmo();

	// ���� ������ ���� ���� ź�෮ ��
	TMap<EWeaponType, int32> CarriedAmmoMap;

	// �ִ� ���� ���� ź�෮
	UPROPERTY(EditAnywhere)
		int32 MaxCarriedAmmo = 500;

	// ���� �� AR ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingARAmmo = 30;

	// ���� �� ���� ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingRocketAmmo = 0;

	// ���� �� �ǽ��� ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingPistolAmmo = 0;

	// ���� �� SMG ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingSMGAmmo = 0;

	// ���� �� ���� ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingShotgunAmmo = 0;

	// ���� �� �������� ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingSniperAmmo = 0;

	// ���� �� ����ź �߻�� ź�෮
	UPROPERTY(EditAnywhere)
		int32 StartingGrenadeLauncherAmmo = 0;


	// ���� ź�� �ʱ�ȭ �Լ�
	void InitializeCarriedAmmo();

	// ���� ���� ����
	// ECombatState::ECS_Unoccupied�� �ʱ�ȭ
	// ���� ������ ��ȭ�� �����ϰ� �����ϱ� ���� ���
	// ���ø����̼� �Ӽ��� ������ �־�, ��Ʈ��ũ ���� �ٸ� ���Ϳ��� ���� ���¸� ����ȭ
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
		ECombatState CombatState = ECombatState::ECS_Unoccupied;

	// ���� ���°� ����� ������ ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnRep_CombatState();

	// ���� ź�� ��ġ ������Ʈ �Լ�
	void UpdateAmmoValues();

	// ���� ź�� ��ġ ������Ʈ �Լ�
	void UpdateShotgunAmmoValues();

	// ����ź �� ����, �ʱ� ���� 4
	// ����ź�� ������ �����ϰ� �����ϱ� ���� ���
	// ���ø����̼� �Ӽ��� ������ �־�, ��Ʈ��ũ ���� �ٸ� ���Ϳ��� ����ź ������ ����ȭ
	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
		int32 Grenades = 4;

	// ����ź ������ ����� ������ ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnRep_Grenades();

	// �ִ� ����ź ����, �⺻���� 4
	UPROPERTY(EditAnywhere)
		int32 MaxGrenades = 4;

	// HUD�� ����ź ���� ������Ʈ �Լ�
	void UpdateHUDGrenades();

	// �÷��� ���� ���� ����
	// �÷��� ���� ���¸� �����ϰ� �����ϱ� ���� ���
	// ���ø����̼� �Ӽ��� ������ �־�, ��Ʈ��ũ ���� �ٸ� ���Ϳ��� �÷��� ���� ���¸� ����ȭ
	UPROPERTY(ReplicatedUsing = OnRep_HoldingTheFlag)
		bool bHoldingTheFlag = false;

	// �÷��� ���� ���ΰ� ����� ������ ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnRep_HoldingTheFlag();

	// ���� ���� �÷��� ��ü
	UPROPERTY()
		AWeapon* TheFlag;

public:
	// ����ź ������ ��ȯ�ϴ� �Լ�
	FORCEINLINE int32 GetGrenades() const { return Grenades; }

	// ���⸦ ��ü�ؾ� �ϴ����� �����ϴ� �Լ�
	bool ShouldSwapWeapons();

};
