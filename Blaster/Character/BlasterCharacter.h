// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/**
	 * ������ ĳ���� Ŭ������ �������Դϴ�.
	 */
	ABlasterCharacter();

	/**
	 * �� �����Ӹ��� ����Ǵ� ������ ����ϴ� �Լ��Դϴ�.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * �÷��̾� �Է� ������ ����ϴ� �Լ��Դϴ�.
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * ��Ʈ��ũ ���ø����̼��� ���� �Ӽ� ������ ����ϴ� �Լ��Դϴ�.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * ������Ʈ �ʱ�ȭ �Ŀ� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	virtual void PostInitializeComponents() override;

	/**
	 * �߻� ��Ÿ�ָ� ����ϴ� �Լ��Դϴ�.
	 * @param bAiming ���� ������ ���θ� ��Ÿ���ϴ�.
	 */
	void PlayFireMontage(bool bAiming);

	/**
	 * ������ ��Ÿ�ָ� ����ϴ� �Լ��Դϴ�.
	 */
	void PlayReloadMontage();

	/**
	 * ���� ��Ÿ�ָ� ����ϴ� �Լ��Դϴ�.
	 */
	void PlayElimMontage();

	/**
	 * ����ź ������ ��Ÿ�ָ� ����ϴ� �Լ��Դϴ�.
	 */
	void PlayThrowGrenadeMontage();

	/**
	 * ���� ��ü ��Ÿ�ָ� ����ϴ� �Լ��Դϴ�.
	 */
	void PlaySwapMontage();

	/**
	 * ReplicatedMovement�� ����� �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	virtual void OnRep_ReplicatedMovement() override;

	/**
	 * �÷��̾ �����ϴ� �Լ��Դϴ�.
	 * @param bPlayerLeftGame �÷��̾ ������ �������� ���θ� ��Ÿ���ϴ�.
	 */
	void Elim(bool bPlayerLeftGame);

	/**
	 * �÷��̾ �����ϴ� �Լ��� ��Ƽĳ��Ʈ�� �����ϴ� �Լ��Դϴ�.
	 * @param bPlayerLeftGame �÷��̾ ������ �������� ���θ� ��Ÿ���ϴ�.
	 */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastElim(bool bPlayerLeftGame);

	/**
	 * ��ü�� ���ŵ� �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	virtual void Destroyed() override;

	/**
	 * �����÷��� ��Ȱ��ȭ ���¸� ��Ÿ���� �����Դϴ�.
	 */
	UPROPERTY(Replicated)
		bool bDisableGameplay = false;

	/**
	 * �������� ������ ������ �����ִ� �̺�Ʈ�Դϴ�.
	 * @param bShowScope �������� ������ ������ �������� ���θ� ��Ÿ���ϴ�.
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void ShowSniperScopeWidget(bool bShowScope);

	/**
	 * HUD�� ü���� ������Ʈ�ϴ� �Լ��Դϴ�.
	 */
	void UpdateHUDHealth();

	/**
	 * HUD�� ���и� ������Ʈ�ϴ� �Լ��Դϴ�.
	 */
	void UpdateHUDShield();

	/**
	 * HUD�� ź���� ������Ʈ�ϴ� �Լ��Դϴ�.
	 */
	void UpdateHUDAmmo();

	/**
	 * �⺻ ���⸦ �����ϴ� �Լ��Դϴ�.
	 */
	void SpawDefaultWeapon();

	/**
	 * ��Ʈ �ڽ��� �����ϴ� ���Դϴ�.
	 */
	UPROPERTY()
		TMap<FName, class UBoxComponent*> HitCollisionBoxes;

	/**
	 * ���� ��ü�� �Ϸ��ߴ��� ���θ� ��Ÿ���� �����Դϴ�.
	 */
	bool bFinishedSwapping = false;

	/**
	 * ���ӿ��� ������ �Լ��� ������ ȣ���ϴ� �Լ��Դϴ�.
	 */
	UFUNCTION(Server, Reliable)
		void ServerLeaveGame();

	/**
	 * ���ӿ��� ������ �� �߻��ϴ� ��������Ʈ�Դϴ�.
	 */
	FOnLeftGame OnLeftGame;

	/**
	 * ������ �Ǿ��� �� ȣ��Ǵ� ��Ƽĳ��Ʈ �Լ��Դϴ�.
	 */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastGainedTheLead();

	/**
	 * �������� �����Ǿ��� �� ȣ��Ǵ� ��Ƽĳ��Ʈ �Լ��Դϴ�.
	 */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastLostTheLead();

	/**
	 * �� ������ �����ϴ� �Լ��Դϴ�.
	 * @param Team ���� ��Ÿ���� ������ �����Դϴ�.
	 */
	void SetTeamColor(ETeam Team);


protected:
	/**
	 * ���� ���� �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	virtual void BeginPlay() override;

	/**
	 * ĳ���͸� ������ �̵���Ű�� �Լ��Դϴ�.
	 * @param Value �̵� ���Դϴ�.
	 */
	void MoveForward(float Value);

	/**
	 * ĳ���͸� ���������� �̵���Ű�� �Լ��Դϴ�.
	 * @param Value �̵� ���Դϴ�.
	 */
	void MoveRight(float Value);

	/**
	 * ĳ������ �¿� ȸ���� ó���ϴ� �Լ��Դϴ�.
	 * @param Value ȸ�� ���Դϴ�.
	 */
	void Turn(float Value);

	/**
	 * ĳ������ ���� �ü��� ó���ϴ� �Լ��Դϴ�.
	 * @param Value �ü� ���Դϴ�.
	 */
	void LookUp(float Value);

	/**
	 * ���� ���� ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void EquipButtonPressed();

	/**
	 * ĳ���� �ɱ� ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void CrouchButtonPressed();

	/**
	 * ������ ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void ReloadButtonPressed();

	/**
	 * ���� ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void AimButtonPressed();

	/**
	 * ���� ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void AimButtonReleased();

	/**
	 * �ð� ����� ���� ���� ��ġ ������ ó���ϴ� �Լ��Դϴ�.
	 * @param DeltaTime ���� �����Ӱ��� �ð� �����Դϴ�.
	 */
	void AimOffset(float DeltaTime);

	/**
	 * AO_Pitch ���� ����ϴ� �Լ��Դϴ�.
	 */
	void CalculateAO_Pitch();

	/**
	 * SimProxy ���� ó���ϴ� �Լ��Դϴ�.
	 */
	void SimProxiesTurn();

	/**
	 * ������ ó���ϴ� �Լ��Դϴ�.
	 */
	virtual void Jump() override;

	/**
	 * �߻� ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void FireButtonPressed();

	/**
	 * �߻� ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void FireButtonReleased();

	/**
	 * �ǰ� ���׼� ��Ÿ�ָ� ����ϴ� �Լ��Դϴ�.
	 */
	void PlayHitReactMontage();

	/**
	 * ����ź ��ư�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void GrenadeButtonPressed();

	/**
	 * ���⸦ ����ϰų� �ı��ϴ� �Լ��Դϴ�.
	 * @param Weapon ����ϰų� �ı��� �����Դϴ�.
	 */
	void DropOrDestroyWeapon(AWeapon* Weapon);

	/**
	 * ��� ���⸦ ����ϰų� �ı��ϴ� �Լ��Դϴ�.
	 */
	void DropOrDestroyWeapons();

	/**
	 * ���� ������ �����ϴ� �Լ��Դϴ�.
	 */
	void SetSpawnPoint();

	/**
	 * �÷��̾� ���°� �ʱ�ȭ�Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void OnPlayerStateInitialized();

	/**
	 * �������� �޾��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 * @param DamagedActor ���ظ� ���� �����Դϴ�.
	 * @param Damage ���� ������ ���Դϴ�.
	 * @param DamageType ������ �����Դϴ�.
	 * @param InstigatorController ���ظ� ���� ��Ʈ�ѷ��Դϴ�.
	 * @param DamageCauser ���ظ� ���� �����Դϴ�.
	 */
	UFUNCTION()
		void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	/**
	 * �ʿ��� Ŭ������ Ȯ���ϰ� HUD�� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
	 */
	void PollInit();

	/**
	 * �÷��̾ ���ڸ����� ȸ���ϴ� ���� ó���ϴ� �Լ��Դϴ�.
	 * @param DeltaTime ���� �����Ӱ��� �ð� �����Դϴ�.
	 */
	void RotateInPlace(float DeltaTime);

	/**
	 * ���� ���̵� �����ε忡 ���Ǵ� ��Ʈ �ڽ��Դϴ�.
	 */
	UPROPERTY(EditAnywhere)
		class UBoxComponent* head;

	UPROPERTY(EditAnywhere)
		UBoxComponent* pelvis;

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_02;

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_03;

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* backpack;

	UPROPERTY(EditAnywhere)
		UBoxComponent* blanket;

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_r;



private:
	/**
	 * ī�޶��� ������ �� ������Ʈ�Դϴ�.
	 */
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	/**
	 * ī�޶� ������Ʈ�Դϴ�.
	 */
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	/**
	 * ĳ������ ���� ǥ�õǴ� ���� ������Ʈ�Դϴ�.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* OverheadWidget;

	/**
	 * ������ ���⸦ ��Ÿ���� �����Դϴ�.
	 */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		class AWeapon* OverlappingWeapon;

	/**
	 * ������ ���Ⱑ ����Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 * @param LastWeapon ������ �����Դϴ�.
	 */
	UFUNCTION()
		void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	/**
	 * Blaster ������Ʈ���Դϴ�.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
		class UBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere)
		class ULagCompensationComponent* LagCompensation;

	/**
	 * AO_Yaw, InterpAO_Yaw, AO_Pitch, StartingAimRotation�� ���� �����Դϴ�.
	 */
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	/**
	 * InPlace ���� ó���ϴ� �Լ��Դϴ�.
	 * @param DeltaTime ���� �����Ӱ��� �ð� �����Դϴ�.
	 */
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	/**
	 * �ִϸ��̼� ��Ÿ�ֵ��Դϴ�.
	 */
	UPROPERTY(EditAnywhere, Category = Combat)
		class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ThrowGrenadeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* SwapMontage;

	/**
	 * ĳ���Ͱ� ������ ���� �� ī�޶� ����� �Լ��Դϴ�.
	 */
	void HideCameraIfCharacterClose();

	/**
	 * ĳ���Ϳ� ī�޶��� �Ÿ� �Ӱ谪�Դϴ�.
	 */
	UPROPERTY(EditAnywhere)
		float CameraThreshold = 200.f;

	bool bRotateRootBone;

	/**
	 * �� �Ӱ谪�Դϴ�.
	 */
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/**
	 * �÷��̾��� ü�� ���� �������Դϴ�.
	 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
		float Health = 100.f;

	/**
	 * ü���� ����Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 * @param LastHealth ������ ü�� ���Դϴ�.
	 */
	UFUNCTION()
		void OnRep_Health(float LastHealth);

	/**
	 * �÷��̾��� �ǵ� ���� �������Դϴ�.
	 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxShield = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
		float Shield = 0.f;

	/**
	 * �ǵ尡 ����Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 * @param LastShield ������ �ǵ� ���Դϴ�.
	 */
	UFUNCTION()
		void OnRep_Shield(float LastShield);

	/**
	 * Blaster �÷��̾� ��Ʈ�ѷ��Դϴ�.
	 */
	UPROPERTY()
		class ABlasterPlayerController* BlasterPlayerController;

	/**
	 * �÷��̾ ���ŵǾ������� ��Ÿ���� �����Դϴ�.
	 */
	bool bElimmed = false;

	FTimerHandle ElimTimer;

	/**
	 * ���� ������ �ð��Դϴ�.
	 */
	UPROPERTY(EditDefaultsOnly)
		float ElimDelay = 3.f;

	/**
	 * ���� Ÿ�̸Ӱ� �Ϸ�Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	 */
	void ElimTimerFinished();

	/**
	 * �÷��̾ ���ӿ��� ���������� ��Ÿ���� �����Դϴ�.
	 */
	bool bLeftGame = false;



/**
 * ������ ȿ���� ���õ� �������Դϴ�.
 */

 // ������ ȿ���� ���Ǵ� Ÿ�Ӷ��� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere)
		UTimelineComponent* DissolveTimeline;

	// ������ ȿ���� ������Ʈ�ϴ� �� ���Ǵ� �Լ��Դϴ�.
	UFUNCTION()
		void UpdateDissolveMaterial(float DissolveValue);

	// ������ ȿ���� �����ϴ� �Լ��Դϴ�.
	void StartDissolve();

	// ��Ÿ�ӿ��� ������ �� �ִ� ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(VisibleAnywhere, Category = Elim)
		UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// �������Ʈ���� ������ �Ұ�(Material Dissolve) ��Ƽ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(VisibleAnywhere, Category = Elim)
		UMaterialInstance* DissolveMaterialInstance;

	/**
	 * �� ����� ���õ� �������Դϴ�.
	 */

	 // ���� ���� �Ұ�(Material Dissolve) ��Ƽ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* RedDissolveMatInst;

	// ���� ���� ��Ƽ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* RedMaterial;

	// �Ķ� ���� �Ұ�(Material Dissolve) ��Ƽ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* BlueDissolveMatInst;

	// �Ķ� ���� ��Ƽ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* BlueMaterial;

	// ���� ��Ƽ���� �ν��Ͻ��Դϴ�.
	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* OriginalMaterial;

	/**
	 * ���� ȿ���� ���õ� �������Դϴ�.
	 */

	 // ���� �κ� ȿ���� ���Ǵ� ��ƼŬ �ý����Դϴ�.
	UPROPERTY(EditAnywhere)
		UParticleSystem* ElimBotEffect;

	// ���� �κ� ȿ���� ���Ǵ� ��ƼŬ �ý��� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ElimBotComponent;

	// ���� �κ� ȿ���� ���Ǵ� ���� ť�Դϴ�.
	UPROPERTY(EditAnywhere)
		class USoundCue* ElimBotSound;

	// Blaster �÷��̾� �����Դϴ�.
	UPROPERTY()
		class ABlasterPlayerState* BlasterPlayerState;

	// �հ� ȿ���� ���Ǵ� Niagara �ý����Դϴ�.
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* CrownSystem;

	// �հ� ȿ���� ���Ǵ� Niagara ������Ʈ�Դϴ�.
	UPROPERTY()
		class UNiagaraComponent* CrownComponent;

	/**
	 * ����ź�� ���õ� �����Դϴ�.
	 */

	 // ĳ���Ϳ� ������ ����ź�� ���� �޽� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* AttachedGrenade;

	/**
	 * �⺻ ����� ���õ� �������Դϴ�.
	 */

	 // �⺻ ���� Ŭ�����Դϴ�.
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeapon> DefaultWeaponClass;

	// Blaster ���� ����Դϴ�.
	UPROPERTY()
		class ABlasterGameMode* BlasterGameMode;



public:
	/**
	 * ���� ��ġ�� ���⸦ �����ϴ� �Լ��Դϴ�.
	 */
	void SetOverlappingWeapon(AWeapon* Weapon);

	/**
	 * ���� ���⸦ ���� ������ Ȯ���ϴ� �Լ��Դϴ�.
	 * @return ���⸦ ���� ������ ����
	 */
	bool IsWeaponEquipped();

	/**
	 * ���� ���� ������ Ȯ���ϴ� �Լ��Դϴ�.
	 * @return ���� ������ ����
	 */
	bool IsAiming();

	/**
	 * Yaw�� ���� Aim Offset ���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return Yaw�� ���� Aim Offset ��
	 */
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	/**
	 * Pitch�� ���� Aim Offset ���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return Pitch�� ���� Aim Offset ��
	 */
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	/**
	 * ���� ������ ���⸦ ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ������ ����
	 */
	AWeapon* GetEquippedWeapon();

	/**
	 * ȸ�� ���� ����(�� ���� ����)�� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ȸ�� ���� ����(�� ���� ����)
	 */
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	/**
	 * ���� ��� ������ ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ���� ��� ����
	 */
	FVector GetHitTarget() const;

	/**
	 * Follow ī�޶� ������Ʈ�� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return Follow ī�޶� ������Ʈ
	 */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/**
	 * Root Bone ȸ�� ���θ� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return Root Bone�� ȸ���ؾ� �ϴ��� ����
	 */
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	/**
	 * ���ŵ� �������� Ȯ���ϴ� �Լ��Դϴ�.
	 * @return ���ŵ� �������� ����
	 */
	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	/**
	 * ���� ü���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ���� ü��
	 */
	FORCEINLINE float GetHealth() const { return Health; }

	/**
	 * ���� ü���� �����ϴ� �Լ��Դϴ�.
	 * @param Amount ������ ü�� ��
	 */
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }

	/**
	 * �ִ� ü���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return �ִ� ü��
	 */
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/**
	 * ���� �ǵ� ���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ���� �ǵ� ��
	 */
	FORCEINLINE float GetShield() const { return Shield; }

	/**
	 * ���� �ǵ� ���� �����ϴ� �Լ��Դϴ�.
	 * @param Amount ������ �ǵ� ��
	 */
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }

	/**
	 * �ִ� �ǵ� ���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return �ִ� �ǵ� ��
	 */
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	/**
	 * ���� ���¸� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ���� ����
	 */
	ECombatState GetCombatState() const;

	/**
	 * ���� ������Ʈ�� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ���� ������Ʈ
	 */
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }

	/**
	 * ���� �÷��� ��Ȱ��ȭ ���θ� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return ���� �÷��� ��Ȱ��ȭ ����
	 */
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	/**
	 * ���ε� ������ Ȯ���ϴ� �Լ��Դϴ�.
	 * @return ���ε� ������ ����
	 */
	bool IsLocallyReloading();

	/**
	 * �� ���� ������Ʈ�� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return �� ���� ������Ʈ
	 */
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }

	/**
	 * ����� ��� �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
	 * @return ����� ��� �ִ��� ����
	 */
	FORCEINLINE bool IsHoldingTheFlag() const;

	/**
	 * �÷��̾��� ���� ��ȯ�ϴ� �Լ��Դϴ�.
	 * @return �÷��̾��� ��
	 */
	ETeam GetTeam();

	/**
	 * ����� ��� �ִ��� ���θ� �����ϴ� �Լ��Դϴ�.
	 * @param bHolding ����� ��� �ִ��� ����
	 */
	void SetHoldingTheFlag(bool bHolding);

};
