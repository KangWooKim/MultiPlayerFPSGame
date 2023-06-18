// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterAnimInstance.h"
#include "Blaster/Weapon/Projectile.h"
#include "Blaster/Weapon/Shotgun.h"

// UCombatComponent Ŭ���� ������
UCombatComponent::UCombatComponent()
{
	// �� ������Ʈ�� ƽ �Լ��� �����ϵ��� ����
	PrimaryComponentTick.bCanEverTick = true;

	// �⺻ �ȱ� �ӵ��� ���� �ȱ� �ӵ� �ʱ�ȭ
	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}

// �����Ǿ�� �� �Ӽ��� �����ϴ� �Լ� (�����Ǵ� �Ӽ�: ������ ����, ���� ����, ���� �� ����, ���� ź��, ���� ����, ����ź, ��� ���� ����)
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UCombatComponent, CombatState);
	DOREPLIFETIME(UCombatComponent, Grenades);
	DOREPLIFETIME(UCombatComponent, bHoldingTheFlag);
}

// ���� ź���� ������Ʈ�ϴ� �޼���
void UCombatComponent::ShotgunShellReload()
{
	// ĳ���Ͱ� �����ϰ�, ������ �ִٸ� ���� ź�� ���� ������Ʈ
	if (Character && Character->HasAuthority())
	{
		UpdateShotgunAmmoValues();
	}

}

// ź�� ȹ�� �޼��� (WeaponType: ���� Ÿ��, AmmoAmount: ȹ���� ź�� ��)
void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	// ȹ���� ���� Ÿ���� ź���� ���� ź�� �ʿ� ���� ���
	if (CarriedAmmoMap.Contains(WeaponType))
	{
		// ���� ź���� �߰��ϰ�, �ִ� ���� �� �ִ� ź�� ���� ���� �ʵ��� ���� ��, ź�� ���� ������Ʈ
		CarriedAmmoMap[WeaponType] = FMath::Clamp(CarriedAmmoMap[WeaponType] + AmmoAmount, 0, MaxCarriedAmmo);
		UpdateCarriedAmmo();
	}
	// ������ ���Ⱑ ���ų�, ���Ⱑ ��� �ְ�, ȹ���� ���� Ÿ�԰� ������ ������ Ÿ���� ���� ���
	if (EquippedWeapon && EquippedWeapon->IsEmpty() && EquippedWeapon->GetWeaponType() == WeaponType)
	{
		// ������
		Reload();
	}
}

// �÷��� ���� �� ȣ��Ǵ� �޼���
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ĳ���Ͱ� �����ϴ� ���
	if (Character)
	{
		// ĳ������ �ִ� �ȱ� �ӵ��� �⺻ �ȱ� �ӵ��� ����
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		// ĳ���Ͱ� ī�޶� ���󰡴� ���
		if (Character->GetFollowCamera())
		{
			// �⺻ �þ� ������ ���� �þ� ���� ����
			DefaultFOV = Character->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
		// ĳ���Ͱ� ������ ���� ���
		if (Character->HasAuthority())
		{
			// ���� ź�� �ʱ�ȭ
			InitializeCarriedAmmo();
		}
	}
}

// ƽ �Լ� (DeltaTime: ���� ������ ������ �ð�, TickType: ƽ ����, ThisTickFunction: �� ƽ �Լ�)
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ĳ���Ͱ� �����ϰ�, ���ÿ��� ����Ǵ� ���
	if (Character && Character->IsLocallyControlled())
	{
		// ������ �Ʒ� ����
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		// HUD ���ڼ� ���� �� �þ� ���� ����
		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}


// �߻� ��ư�� ���ȴ��� Ȯ���ϴ� �޼��� (bPressed: �߻� ��ư�� ���ȴ��� ����)
void UCombatComponent::FireButtonPressed(bool bPressed)
{
	// �߻� ��ư�� ���ȴ��� ���θ� ������Ʈ
	bFireButtonPressed = bPressed;
	// �߻� ��ư�� ���� ���, �߻� �޼��带 ȣ��
	if (bFireButtonPressed)
	{
		Fire();
	}
}

// ���⸦ �߻��ϴ� �޼���
void UCombatComponent::Fire()
{
	// �߻� ������ ��쿡�� �߻�
	if (CanFire())
	{
		bCanFire = false;
		if (EquippedWeapon)
		{
			CrosshairShootingFactor = .75f;

			// ������ ������ �߻� ������ ���� ������ �߻� �޼��带 ȣ��
			switch (EquippedWeapon->FireType)
			{
			case EFireType::EFT_Projectile:
				FireProjectileWeapon();
				break;
			case EFireType::EFT_HitScan:
				FireHitScanWeapon();
				break;
			case EFireType::EFT_Shotgun:
				FireShotgun();
				break;
			}
		}
		// �߻� Ÿ�̸� ����
		StartFireTimer();
	}
}

// �߻�ü ���⸦ �߻��ϴ� �޼���
void UCombatComponent::FireProjectileWeapon()
{
	if (EquippedWeapon && Character)
	{
		// Scatter �ɼǿ� ���� Ÿ�� ��ġ�� ���
		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		if (!Character->HasAuthority()) LocalFire(HitTarget);
		// �������� �߻� ó��
		ServerFire(HitTarget, EquippedWeapon->FireDelay);
	}
}

// ��Ʈ��ĵ ���⸦ �߻��ϴ� �޼���
void UCombatComponent::FireHitScanWeapon()
{
	if (EquippedWeapon && Character)
	{
		// Scatter �ɼǿ� ���� Ÿ�� ��ġ�� ���
		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		if (!Character->HasAuthority()) LocalFire(HitTarget);
		// �������� �߻� ó��
		ServerFire(HitTarget, EquippedWeapon->FireDelay);
	}
}

// ������ �߻��ϴ� �޼���
void UCombatComponent::FireShotgun()
{
	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
	if (Shotgun && Character)
	{
		TArray<FVector_NetQuantize> HitTargets;
		// ������ ���� Ÿ�ٿ� Ÿ���� �� �ֵ��� ó��
		Shotgun->ShotgunTraceEndWithScatter(HitTarget, HitTargets);
		if (!Character->HasAuthority()) ShotgunLocalFire(HitTargets);
		// �������� �߻� ó��
		ServerShotgunFire(HitTargets, EquippedWeapon->FireDelay);
	}
}

// �߻� Ÿ�̸Ӹ� �����ϴ� �޼���
void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	// �߻� ���� ���� Ÿ�̸Ӹ� �����ϰ�, Ÿ�̸Ӱ� ������ FireTimerFinished �޼��� ȣ��
	Character->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay
	);
}
// �߻� Ÿ�̸Ӱ� ������ ���� ó���� �ϴ� �޼���
void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;
	// �߻� ��ư�� ��� ������ �ְ�, ���Ⱑ �ڵ� �߻����� ��� �ٽ� �߻�
	if (bFireButtonPressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}
	// ���Ⱑ ��������� ������
	ReloadEmptyWeapon();
}

// �������� �߻縦 ó���ϴ� �޼��� (TraceHitTarget: Ÿ�� ��ġ, FireDelay: �߻� ������)
void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	// ��� Ŭ���̾�Ʈ���� �߻縦 �˸�
	MulticastFire(TraceHitTarget);
}

// �������� �߻縦 �����ϴ� �޼��� (TraceHitTarget: Ÿ�� ��ġ, FireDelay: �߻� ������)
bool UCombatComponent::ServerFire_Validate(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	if (EquippedWeapon)
	{
		// ������ �߻� �����̿� �Ķ������ �߻� �����̰� ���� ������ ����
		bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->FireDelay, FireDelay, 0.001f);
		return bNearlyEqual;
	}
	return true;
}

// ��� Ŭ���̾�Ʈ���� �߻縦 �˸��� �޼��� (TraceHitTarget: Ÿ�� ��ġ)
void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	// ���� ��Ʈ���̰� ������ ���� Ŭ���̾�Ʈ�� ����
	if (Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;
	LocalFire(TraceHitTarget);
}

// �������� ���� �߻縦 ó���ϴ� �޼��� (TraceHitTargets: ���� Ÿ�� ��ġ��, FireDelay: �߻� ������)
void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	// ��� Ŭ���̾�Ʈ���� ���� �߻縦 �˸�
	MulticastShotgunFire(TraceHitTargets);
}

// �������� ���� �߻縦 �����ϴ� �޼��� (TraceHitTargets: ���� Ÿ�� ��ġ��, FireDelay: �߻� ������)
bool UCombatComponent::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	if (EquippedWeapon)
	{
		// ������ �߻� �����̿� �Ķ������ �߻� �����̰� ���� ������ ����
		bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->FireDelay, FireDelay, 0.001f);
		return bNearlyEqual;
	}
	return true;
}

// ��� Ŭ���̾�Ʈ���� ���� �߻縦 �˸��� �޼��� (TraceHitTargets: ���� Ÿ�� ��ġ��)
void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	// ���� ��Ʈ���̰� ������ ���� Ŭ���̾�Ʈ�� ����
	if (Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;
	ShotgunLocalFire(TraceHitTargets);
}

// ���ÿ��� �߻縦 ó���ϴ� �޼��� (TraceHitTarget: Ÿ�� ��ġ)
void UCombatComponent::LocalFire(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr) return;
	if (Character && CombatState == ECombatState::ECS_Unoccupied)
	{
		// �߻� �ִϸ��̼� ��� �� ���� �߻�
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}
// ������ ���� �߻縦 ó���ϴ� �޼��� (TraceHitTargets: ���� Ÿ�� ��ġ��)
void UCombatComponent::ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);  // ���� ������ ���Ⱑ �������� Ȯ��
	if (Shotgun == nullptr || Character == nullptr) return;
	if (CombatState == ECombatState::ECS_Reloading || CombatState == ECombatState::ECS_Unoccupied)
	{
		bLocallyReloading = false;
		Character->PlayFireMontage(bAiming);
		Shotgun->FireShotgun(TraceHitTargets);  // ���� �߻� ó��
		CombatState = ECombatState::ECS_Unoccupied;
	}
}

// ���� ���� ó���� �ϴ� �޼��� (WeaponToEquip: ������ ����)
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (WeaponToEquip->GetWeaponType() == EWeaponType::EWT_Flag)
	{
		Character->Crouch();
		bHoldingTheFlag = true;
		WeaponToEquip->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachFlagToLeftHand(WeaponToEquip);  // ����� �޼տ� ����
		WeaponToEquip->SetOwner(Character);
		TheFlag = WeaponToEquip;
	}
	else
	{
		if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
		{
			EquipSecondaryWeapon(WeaponToEquip);  // ���� ���� ���� ó��
		}
		else
		{
			EquipPrimaryWeapon(WeaponToEquip);  // ���� ���� ���� ó��
		}

		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

// ���� ��ü ó���� �ϴ� �޼���
void UCombatComponent::SwapWeapons()
{
	if (CombatState != ECombatState::ECS_Unoccupied || Character == nullptr || !Character->HasAuthority()) return;

	Character->PlaySwapMontage();
	CombatState = ECombatState::ECS_SwappingWeapons;
	Character->bFinishedSwapping = false;
	if (SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(false);
}

// �� ���� ���� ó���� �ϴ� �޼��� (WeaponToEquip: ������ ����)
void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	DropEquippedWeapon();  // ���� ������ ���� ���
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorToRightHand(EquippedWeapon);  // ���⸦ �����տ� ����
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetHUDAmmo();  // HUD�� ź�� ǥ�� ������Ʈ
	UpdateCarriedAmmo();
	PlayEquipWeaponSound(WeaponToEquip);
	ReloadEmptyWeapon();
}

// �� ���� ���� ó���� �ϴ� �޼��� (WeaponToEquip: ������ ����)
void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackpack(WeaponToEquip);  // ���⸦ ���ѿ� ����
	PlayEquipWeaponSound(WeaponToEquip);
	SecondaryWeapon->SetOwner(Character);
}
// 'Aiming' �Ӽ� ������ ó���ϴ� �޼���
void UCombatComponent::OnRep_Aiming()
{
	// ĳ���Ͱ� ���ÿ��� ����Ǵ� ��쿡�� ���� ������ ������Ʈ
	if (Character && Character->IsLocallyControlled())
	{
		bAiming = bAimButtonPressed;
	}
}

// ���� ������ ���⸦ ����ϴ� �޼���
void UCombatComponent::DropEquippedWeapon()
{
	// ���� ������ ���Ⱑ ������ ���
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
}

// �����տ� ��츦 �����ϴ� �޼��� (ActorToAttach: ������ ���)
void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	// ĳ���� �Ǵ� ĳ������ �Ž�, ������ ��찡 ������ ����
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		// ������ ���Ͽ� ��� ����
		HandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

// �޼տ� ����� �����ϴ� �޼��� (Flag: ������ ���)
void UCombatComponent::AttachFlagToLeftHand(AWeapon* Flag)
{
	// ĳ���� �Ǵ� ĳ������ �Ž�, ������ ����� ������ ����
	if (Character == nullptr || Character->GetMesh() == nullptr || Flag == nullptr) return;
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("FlagSocket"));
	if (HandSocket)
	{
		// ��� ���Ͽ� ��� ����
		HandSocket->AttachActor(Flag, Character->GetMesh());
	}
}

// �޼տ� ��츦 �����ϴ� �޼��� (ActorToAttach: ������ ���)
void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach)
{
	// ĳ���� �Ǵ� ĳ������ �Ž�, ������ ���, ������ ���Ⱑ ������ ����
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr || EquippedWeapon == nullptr) return;
	bool bUsePistolSocket =
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol ||
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SubmachineGun;
	FName SocketName = bUsePistolSocket ? FName("PistolSocket") : FName("LeftHandSocket");
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(SocketName);
	if (HandSocket)
	{
		// ���õ� ���Ͽ� ��� ����
		HandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

// ���ѿ� ��츦 �����ϴ� �޼��� (ActorToAttach: ������ ���)
void UCombatComponent::AttachActorToBackpack(AActor* ActorToAttach)
{
	// ĳ���� �Ǵ� ĳ������ �Ž�, ������ ��찡 ������ ����
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* BackpackSocket = Character->GetMesh()->GetSocketByName(FName("BackpackSocket"));
	if (BackpackSocket)
	{
		// ���� ���Ͽ� ��� ����
		BackpackSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

// �����ϰ� �ִ� ź���� ������Ʈ�ϴ� �޼���
void UCombatComponent::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr) return;
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		// HUD�� ���� ź�෮ ǥ�� ������Ʈ
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

// ���� ���� ���带 ����ϴ� �޼��� (WeaponToEquip: ������ ����)
void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip)
{
	if (Character && WeaponToEquip && WeaponToEquip->EquipSound)
	{
		// ���� ���� ���
		UGameplayStatics::PlaySoundAtLocation(
			this,
			WeaponToEquip->EquipSound,
			Character->GetActorLocation()
		);
	}
}
// �� ���⸦ �������ϴ� �޼���
void UCombatComponent::ReloadEmptyWeapon()
{
	// ���� ������ ���Ⱑ �ְ�, �� ���Ⱑ �� ��� ������
	if (EquippedWeapon && EquippedWeapon->IsEmpty())
	{
		Reload();
	}
}

// ���⸦ �������ϴ� �޼���
void UCombatComponent::Reload()
{
	// ���� ������ ź���� �ְ�, ���� ���°� '��Ȱ��'�̸�, ������ ���Ⱑ �ְ�, �� ���Ⱑ ���� ���� �ʾ�����, ���ÿ��� ������ ���� �ƴ� ���
	if (CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon && !EquippedWeapon->IsFull() && !bLocallyReloading)
	{
		ServerReload();
		HandleReload();
		bLocallyReloading = true; // ������ ������ ���� ����
	}
}

// �������� �������� ó���ϴ� �޼���
void UCombatComponent::ServerReload_Implementation()
{
	// ĳ���� �Ǵ� ������ ���Ⱑ ���� ��� ����
	if (Character == nullptr || EquippedWeapon == nullptr) return;

	CombatState = ECombatState::ECS_Reloading; // ���� ���¸� '������ ��'���� ����
	if (!Character->IsLocallyControlled()) HandleReload();
}

// �������� �Ϸ��ϴ� �޼���
void UCombatComponent::FinishReloading()
{
	if (Character == nullptr) return;
	bLocallyReloading = false; // ������ �� ���� ����
	if (Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied; // ���� ���¸� '��Ȱ��'���� ����
		UpdateAmmoValues(); // ź�� �� ������Ʈ
	}
	if (bFireButtonPressed)
	{
		Fire(); // �߻� ��ư�� ���� ��� �߻�
	}
}

// ������ �Ϸ��ϴ� �޼���
void UCombatComponent::FinishSwap()
{
	if (Character && Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied; // ���� ���¸� '��Ȱ��'���� ����
	}
	if (Character) Character->bFinishedSwapping = true; // ĳ������ ���� �Ϸ� ���� ����
	if (SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(true);
}

// ���� �� ���⸦ �����ϴ� �޼���
void UCombatComponent::FinishSwapAttachWeapons()
{
	PlayEquipWeaponSound(SecondaryWeapon); // ���� ���� ���� ���

	if (Character == nullptr || !Character->HasAuthority()) return;
	AWeapon* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped); // ������ ���� ���� ����
	AttachActorToRightHand(EquippedWeapon); // �����տ� ���� ����
	EquippedWeapon->SetHUDAmmo(); // HUD�� ź�� ����
	UpdateCarriedAmmo(); // ������ ź�� ������Ʈ

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary); // ������ �� ��° ������ ���� ����
	AttachActorToBackpack(SecondaryWeapon); // ���ѿ� �� ��° ���� ����
}

// ź�� ���� ������Ʈ�ϴ� �޼���
void UCombatComponent::UpdateAmmoValues()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	int32 ReloadAmount = AmountToReload(); // �������� ź�� ����
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo); // HUD�� ���� ź�෮ ǥ�� ������Ʈ
	}
	EquippedWeapon->AddAmmo(ReloadAmount); // ������ ���⿡ ź�� �߰�
}
// ������ ź�� ���� ������Ʈ�ϴ� �޼���
void UCombatComponent::UpdateShotgunAmmoValues()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;

	// ������ ���� Ÿ�Կ� �ش��ϴ� ź���� �ִٸ� �� ź���� 1��ŭ ����
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= 1;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	// ��Ʈ�ѷ��� ���ٸ� ĳ������ ��Ʈ�ѷ��� ����
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);  // HUD�� ź�� ���� ������Ʈ
	}

	EquippedWeapon->AddAmmo(1);  // ������ ���⿡ ź�� �߰�
	bCanFire = true;  // �߻� ���� ���·� ����

	if (EquippedWeapon->IsFull() || CarriedAmmo == 0)
	{
		JumpToShotgunEnd();  // ������ ���� ���ų� ź���� ���ٸ� ������ ����
	}
}

// ����ź ������ ������� �� HUD�� ������Ʈ�ϴ� �޼���
void UCombatComponent::OnRep_Grenades()
{
	UpdateHUDGrenades();
}

// ���� �������� ������ �κ����� �̵��ϴ� �޼���
void UCombatComponent::JumpToShotgunEnd()
{
	// Jump to ShotgunEnd section
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance && Character->GetReloadMontage())
	{
		AnimInstance->Montage_JumpToSection(FName("ShotgunEnd"));  // �ִϸ��̼��� ���� ������ ���� �������� �̵�
	}
}

// ����ź ��ô�� �Ϸ�� �� ó���ϴ� �޼���
void UCombatComponent::ThrowGrenadeFinished()
{
	CombatState = ECombatState::ECS_Unoccupied;  // ���� ���¸� '��Ȱ��'���� ����
	AttachActorToRightHand(EquippedWeapon);  // �����տ� ���⸦ ����
}

// ����ź�� ��ô�ϴ� �޼���
void UCombatComponent::LaunchGrenade()
{
	ShowAttachedGrenade(false);  // ������ ����ź ����
	if (Character && Character->IsLocallyControlled())
	{
		ServerLaunchGrenade(HitTarget);  // �������� ����ź ��ô ó��
	}
}

// �������� ����ź�� ��ô�ϴ� �޼���, �Ķ���ͷ� ��ô ��� ��ġ�� ����
void UCombatComponent::ServerLaunchGrenade_Implementation(const FVector_NetQuantize& Target)
{
	if (Character && GrenadeClass && Character->GetAttachedGrenade())
	{
		const FVector StartingLocation = Character->GetAttachedGrenade()->GetComponentLocation();  // ��ô ���� ��ġ�� ����ź ��ġ�� ����
		FVector ToTarget = Target - StartingLocation;  // ��ǥ ������ ���ϴ� ���� ���
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Character;
		SpawnParams.Instigator = Character;
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AProjectile>(
				GrenadeClass,
				StartingLocation,
				ToTarget.Rotation(),
				SpawnParams
				);  // ���迡 ����ź ���� ����
		}
	}
}

// ���� ���� ���濡 ���� ó���� �ϴ� �޼���
void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Reloading:
		if (Character && !Character->IsLocallyControlled()) HandleReload();  // ������ ó��
		break;
	case ECombatState::ECS_Unoccupied:
		if (bFireButtonPressed)
		{
			Fire();  // �߻� ó��
		}
		break;
	case ECombatState::ECS_ThrowingGrenade:
		if (Character && !Character->IsLocallyControlled())
		{
			Character->PlayThrowGrenadeMontage();  // ����ź ��ô �ִϸ��̼� ���
			AttachActorToLeftHand(EquippedWeapon);  // �޼տ� ���� ����
			ShowAttachedGrenade(true);  // ����ź ���̰� ����
		}
		break;
	case ECombatState::ECS_SwappingWeapons:
		if (Character && !Character->IsLocallyControlled())
		{
			Character->PlaySwapMontage();  // ���� ��ü �ִϸ��̼� ���
		}
		break;
	}
}
// �������� ó���ϴ� �޼���
void UCombatComponent::HandleReload()
{
	if (Character)
	{
		Character->PlayReloadMontage();  // ������ �ִϸ��̼� ���
	}
}

// �������ؾ� �� ź���� ���� ��ȯ�ϴ� �޼���
int32 UCombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;  // ������ ���Ⱑ ���ٸ� 0 ��ȯ
	int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();  // źâ�� ���� ���� ���

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))  // ������ �ִ� ź�� Ÿ���� ������ ������ Ÿ�԰� ��ġ�ϴ� ���
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMag, AmountCarried);  // źâ�� ���� ������ ������ �ִ� ź�� �� �ּҰ� ���
		return FMath::Clamp(RoomInMag, 0, Least);  // �������ؾ� �� ź���� �� ��ȯ
	}
	return 0;
}

// ����ź�� ������ �޼���
void UCombatComponent::ThrowGrenade()
{
	if (Grenades == 0) return;  // ����ź�� ���ٸ� ��ȯ
	if (CombatState != ECombatState::ECS_Unoccupied || EquippedWeapon == nullptr) return;  // ���� ���°� ��Ȱ�� ���°� �ƴϰų�, ������ ���Ⱑ ���ٸ� ��ȯ
	CombatState = ECombatState::ECS_ThrowingGrenade;  // ���� ���¸� ����ź ������ ���·� ����
	if (Character)
	{
		Character->PlayThrowGrenadeMontage();  // ����ź ������ �ִϸ��̼� ���
		AttachActorToLeftHand(EquippedWeapon);  // �޼տ� ���� ����
		ShowAttachedGrenade(true);  // ����ź ���̰� ����
	}
	if (Character && !Character->HasAuthority())
	{
		ServerThrowGrenade();  // �������� ����ź ������ �޼��� ȣ��
	}
	if (Character && Character->HasAuthority())
	{
		Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);  // ����ź ���� ����
		UpdateHUDGrenades();  // HUD�� ����ź ���� ������Ʈ
	}
}

// �������� ����ź�� ������ �޼���
void UCombatComponent::ServerThrowGrenade_Implementation()
{
	if (Grenades == 0) return;  // ����ź�� ���ٸ� ��ȯ
	CombatState = ECombatState::ECS_ThrowingGrenade;  // ���� ���¸� ����ź ������ ���·� ����
	if (Character)
	{
		Character->PlayThrowGrenadeMontage();  // ����ź ������ �ִϸ��̼� ���
		AttachActorToLeftHand(EquippedWeapon);  // �޼տ� ���� ����
		ShowAttachedGrenade(true);  // ����ź ���̰� ����
	}
	Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);  // ����ź ���� ����
	UpdateHUDGrenades();  // HUD�� ����ź ���� ������Ʈ
}

// HUD�� ����ź ������ ������Ʈ�ϴ� �޼���
void UCombatComponent::UpdateHUDGrenades()
{
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;  // ��Ʈ�ѷ� ����
	if (Controller)
	{
		Controller->SetHUDGrenades(Grenades);  // HUD�� ����ź ���� ����
	}
}

// ���⸦ ��ü�ؾ� �ϴ��� ���θ� �Ǵ��ϴ� �޼���
bool UCombatComponent::ShouldSwapWeapons()
{
	return (EquippedWeapon != nullptr && SecondaryWeapon != nullptr);  // ������ ����� ������ ���� �� �� �ִ� ��쿡�� true ��ȯ
}

// ����ź ���̰� �����ϴ� �޼���
// bShowGrenade: ����ź�� ���̰� ���� ����
void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade)
{
	if (Character && Character->GetAttachedGrenade())
	{
		Character->GetAttachedGrenade()->SetVisibility(bShowGrenade);  // ����ź ���̰� ����
	}
}

// ������ ���� ���� �� ó���ϴ� �޼���
void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);  // ���� ���¸� ������ ���·� ����
		AttachActorToRightHand(EquippedWeapon);  // �����տ� ���� ����
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;  // ĳ���� ȸ�� ���� ����
		Character->bUseControllerRotationYaw = true;  // ��Ʈ�ѷ��� ȸ�� ���� ����
		PlayEquipWeaponSound(EquippedWeapon);  // ���� ���� ���� ���
		EquippedWeapon->EnableCustomDepth(false);  // Ŀ���� ���� ��Ȱ��ȭ
		EquippedWeapon->SetHUDAmmo();  // HUD�� ź�� ����
	}
}


// ���� ���Ⱑ �����Ǿ��� �� ó���ϴ� �޼���
void UCombatComponent::OnRep_SecondaryWeapon()
{
	if (SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);  // ���� ������ ���¸� '���� ����� ������'���� ����
		AttachActorToBackpack(SecondaryWeapon);  // ���� ���⸦ ���ѿ� ����
		PlayEquipWeaponSound(EquippedWeapon);  // ���� ���� ���� ���
	}
}

// ���ؼ� �Ʒ��� �ִ� ��ü�� �����ϴ� �޼���
// TraceHitResult: ��ü ���� ���
void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);  // ����Ʈ ũ�� ��������
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);  // ���ؼ� ��ġ ����
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(  // ��ũ������ ���� ��ǥ�� ��ȯ
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)  // ��ũ������ ���� ��ǥ�� ��ȯ�� �����ߴٸ�
	{
		FVector Start = CrosshairWorldPosition;  // ������ ����

		if (Character)  // ĳ���Ͱ� �ִٸ�
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();  // ĳ���Ϳ��� �Ÿ� ���
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);  // ������ ����
		}

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;  // ���� ����

		GetWorld()->LineTraceSingleByChannel(  // �� ���� ����
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())  // ���� ������� ��ü�� �����, �� ��ü�� UInteractWithCrosshairsInterface�� �����Ѵٸ�
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;  // ���ؼ� ������ ���������� ����
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;  // ���ؼ� ������ ������� ����
		}
	}
}

// HUD�� ���ؼ��� �����ϴ� �޼���
// DeltaTime: ���� �����ӿ��� ���� �����ӱ����� �ð�
void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	// ĳ���ͳ� ��Ʈ�ѷ��� ���ٸ� �Լ� ����
	if (Character == nullptr || Character->Controller == nullptr) return;

	// ��Ʈ�ѷ� ����
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		// HUD ����
		HUD = HUD == nullptr ? Cast<ABlasterHUD>(Controller->GetHUD()) : HUD;
		if (HUD)
		{
			// ������ ���Ⱑ �ִٸ�
			if (EquippedWeapon)
			{
				// ���ؼ� ��ġ�� ������ ��ġ�� ���� ����
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
			}
			else
			{
				// ���Ⱑ ���ٸ� ���ؼ� ��ġ�� null�� ����
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
			}

			// ���ؼ� ���� ���
			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;

			// �ӵ��� ���� ���ؼ� ��� ���
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			// ĳ���Ͱ� �������� �ִٸ�
			if (Character->GetCharacterMovement()->IsFalling())
			{
				// ���߿� �ִ� ����� ���ؼ� ��Ҹ� ���
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				// ���߿� ���� ����� ���ؼ� ��Ҹ� ���
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			// ���� ���̶��
			if (bAiming)
			{
				// ���� ���� ���� ���ؼ� ��Ҹ� ���
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				// ���� ���� �ƴ� ���� ���ؼ� ��Ҹ� ���
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			// ���� �� ���� ���ؼ� ��Ҹ� ���
			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			// ��� ��Ҹ� ���Ͽ� ���ؼ� ���� ���
			HUDPackage.CrosshairSpread =
				0.5f +
				CrosshairVelocityFactor +
				CrosshairInAirFactor -
				CrosshairAimFactor +
				CrosshairShootingFactor;

			// HUD�� ��Ű�� ����
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

// DeltaTime : ���� �����ӿ��� ���� �����ӱ����� �ð�
void UCombatComponent::InterpFOV(float DeltaTime)
{
	// ���Ⱑ ���ٸ� �Լ� ����
	if (EquippedWeapon == nullptr) return;

	// ���� ���̶��
	if (bAiming)
	{
		// ���� ���� ����� �þ� ���� ���
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
	}
	else
	{
		// ���� ���� �ƴ� ����� �þ� ���� ���
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	// ĳ���Ϳ� ���� ī�޶� �ִٸ�
	if (Character && Character->GetFollowCamera())
	{
		// ī�޶��� �þ� ���� ����
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}


// bIsAiming : ���� �������� ��Ÿ���� ����
void UCombatComponent::SetAiming(bool bIsAiming)
{
	// ĳ���ͳ� ������ ���Ⱑ ���ٸ� �Լ� ����
	if (Character == nullptr || EquippedWeapon == nullptr) return;

	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (Character)
	{
		// ĳ������ �ִ� �̵� �ӵ��� ���� ������ ���ο� ���� ����
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
	if (Character->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		// ���� ���̰� ������ ���Ⱑ �������� �������� ��� �������� ������ ���� ǥ��
		Character->ShowSniperScopeWidget(bIsAiming);
	}
	if (Character->IsLocallyControlled()) bAimButtonPressed = bIsAiming;
}

// bIsAiming : ���� �������� ��Ÿ���� ����
void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		// ĳ������ �ִ� �̵� �ӵ��� ���� ������ ���ο� ���� ����
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

bool UCombatComponent::CanFire()
{
	// ���Ⱑ ���ų� ���Ⱑ ��� �ְų� ���� �� �� ���ų� ������ ���� ��� �Ұ���
	if (EquippedWeapon == nullptr) return false;
	if (!EquippedWeapon->IsEmpty() && bCanFire && CombatState == ECombatState::ECS_Reloading && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun) return true;
	if (bLocallyReloading) return false;
	// �� ���� ��쿡�� ���Ⱑ ��� ���� �ʰ� ���� �� �� ������ ���� ���°� ��� ���� ��� ����
	return !EquippedWeapon->IsEmpty() && bCanFire && CombatState == ECombatState::ECS_Unoccupied;
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	// ��Ʈ�ѷ��� �����ϰ�, HUD�� ź�� ���� ����
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
	bool bJumpToShotgunEnd =
		CombatState == ECombatState::ECS_Reloading &&
		EquippedWeapon != nullptr &&
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun &&
		CarriedAmmo == 0;
	if (bJumpToShotgunEnd)
	{
		JumpToShotgunEnd();
	}
}

void UCombatComponent::InitializeCarriedAmmo()
{
	// �� ���� �������� �ʱ� ź�� ���� ����
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SubmachineGun, StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher, StartingGrenadeLauncherAmmo);
}

void UCombatComponent::OnRep_HoldingTheFlag()
{
	// �÷��׸� ��� �ְ�, ĳ���Ͱ� ���÷� ����Ǵ� ��� ĳ���͸� ��ũ���� ��
	if (bHoldingTheFlag && Character && Character->IsLocallyControlled())
	{
		Character->Crouch();
	}
}
