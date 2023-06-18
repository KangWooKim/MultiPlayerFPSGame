// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// ������
UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // �� ƽ ���� ����
}


// ü�� ȸ�� �޼���
// HealAmount: ȸ���� ü�·�
// HealingTime: ü�� ȸ���� �ɸ� �ð�
void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true; // ȸ�� ���·� ����
	HealingRate = HealAmount / HealingTime; // �ʴ� ü�� ȸ���� ����
	AmountToHeal += HealAmount; // �� ȸ���� ü�·� ����
}

// �� ���� �޼���
// ShieldAmount: ������ ����
// ReplenishTime: �� ���濡 �ɸ� �ð�
void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	bReplenishingShield = true; // �� ���� ���·� ����
	ShieldReplenishRate = ShieldAmount / ReplenishTime; // �ʴ� �� ����� ����
	ShieldReplenishAmount += ShieldAmount; // �� ������ ���� ����
}

// ü���� �ð��� ���� ������ ȸ����Ű�� �޼���
// DeltaTime: ������ �� �ð� ����
void UBuffComponent::HealRampUp(float DeltaTime)
{
	if (!bHealing || Character == nullptr || Character->IsElimmed()) return; // ȸ�� ���°� �ƴϰų� ĳ���Ͱ� ���ų� ����� ��� ����

	const float HealThisFrame = HealingRate * DeltaTime; // �̹� �����ӿ��� ȸ���� ü�·�
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth())); // ü�� ����, �ִ� ü���� ���� �ʵ��� Ŭ����
	Character->UpdateHUDHealth(); // HUD�� ü�� ǥ�� ����
	AmountToHeal -= HealThisFrame; // ���� ȸ���� ü�·� ����

	if (AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth()) // ��� ü���� ȸ���ϰų� ĳ������ ü���� �ִ뿡 ������ ���
	{
		bHealing = false; // ȸ�� ���� ����
		AmountToHeal = 0.f; // ���� ȸ���� ü�·� �ʱ�ȭ
	}
}

// ���� �ð��� ���� ������ �����Ű�� �޼���
// DeltaTime: ������ �� �ð� ����
void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	if (!bReplenishingShield || Character == nullptr || Character->IsElimmed()) return; // �� ���� ���°� �ƴϰų� ĳ���Ͱ� ���ų� ����� ��� ����

	const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime; // �̹� �����ӿ��� ������ ����
	Character->SetShield(FMath::Clamp(Character->GetShield() + ReplenishThisFrame, 0.f, Character->GetMaxShield())); // �� ����, �ִ� ���� ���� �ʵ��� Ŭ����
	Character->UpdateHUDShield(); // HUD�� �� ǥ�� ����
	ShieldReplenishAmount -= ReplenishThisFrame; // ���� ������ ���� ����

	if (ShieldReplenishAmount <= 0.f || Character->GetShield() >= Character->GetMaxShield()) // ��� ���� �����ϰų� ĳ������ ���� �ִ뿡 ������ ���
	{
		bReplenishingShield = false; // �� ���� ���� ����
		ShieldReplenishAmount = 0.f; // ���� ������ ���� �ʱ�ȭ
	}
}

// BeginPlay �޼���
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay(); // �θ� Ŭ������ BeginPlay ȣ��
}


// �ʱ� �̵� �ӵ� ���� �޼���
// BaseSpeed: �⺻ �̵� �ӵ�
// CrouchSpeed: ���� ���¿����� �̵� �ӵ�
void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed; // �⺻ �̵� �ӵ� ����
	InitialCrouchSpeed = CrouchSpeed; // ���� ���¿����� �̵� �ӵ� ����
}

// �ʱ� ���� �ӵ� ���� �޼���
// Velocity: ���� �ӵ�
void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity; // ���� �ӵ� ����
}

// �̵� �ӵ� ��ȭ �޼���
// BuffBaseSpeed: ��ȭ�� �⺻ �̵� �ӵ�
// BuffCrouchSpeed: ��ȭ�� ���� ���¿����� �̵� �ӵ�
// BuffTime: �ӵ� ��ȭ ���� �ð�
void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if (Character == nullptr) return; // ĳ���Ͱ� ���� ��� ��ȯ

	// BuffTime �Ŀ� ResetSpeeds �޼��� ������ ����
	Character->GetWorldTimerManager().SetTimer(
		SpeedBuffTimer,
		this,
		&UBuffComponent::ResetSpeeds,
		BuffTime
	);

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BuffBaseSpeed; // �⺻ �̵� �ӵ� ��ȭ
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed; // ���� ���¿����� �̵� �ӵ� ��ȭ
	}
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed); // �ٸ� Ŭ���̾�Ʈ�鿡�Ե� �̵� �ӵ� ��ȭ �˸�
}

// �̵� �ӵ� �ʱ�ȭ �޼���
void UBuffComponent::ResetSpeeds()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr) return; // ĳ���Ͱ� ���ų� ĳ���� �̵� ������Ʈ�� ���� ��� ��ȯ

	Character->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed; // �⺻ �̵� �ӵ� �ʱ�ȭ
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed; // ���� ���¿����� �̵� �ӵ� �ʱ�ȭ
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed); // �ٸ� Ŭ���̾�Ʈ�鿡�Ե� �̵� �ӵ� �ʱ�ȭ �˸�
}

// �̵� �ӵ� ��ȭ �˸� �޼��� (�ٸ� Ŭ���̾�Ʈ�鿡�� �˸�)
// BaseSpeed: �⺻ �̵� �ӵ�
// CrouchSpeed: ���� ���¿����� �̵� �ӵ�
void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed; // �⺻ �̵� �ӵ� ����
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed; // ���� ���¿����� �̵� �ӵ� ����
	}
}

// ���� ��ȭ �޼���
// BuffJumpVelocity: ��ȭ�� ���� �ӵ�
// BuffTime: ���� ��ȭ ���� �ð�
void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (Character == nullptr) return; // ĳ���Ͱ� ���� ��� ��ȯ

	// BuffTime �Ŀ� ResetJump �޼��� ������ ����
	Character->GetWorldTimerManager().SetTimer(
		JumpBuffTimer,
		this,
		&UBuffComponent::ResetJump,
		BuffTime
	);

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = BuffJumpVelocity; // ���� �ӵ� ��ȭ
	}
	MulticastJumpBuff(BuffJumpVelocity); // �ٸ� Ŭ���̾�Ʈ�鿡�Ե� ���� �ӵ� ��ȭ �˸�
}

// ���� ��ȭ �˸� �޼��� (�ٸ� Ŭ���̾�Ʈ�鿡�� �˸�)
// JumpVelocity: ���� �ӵ�
void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity; // ���� �ӵ� ����
	}
}


// ���� �ʱ�ȭ �޼���
void UBuffComponent::ResetJump()
{
	if (Character->GetCharacterMovement()) // ĳ���� �̵� ������Ʈ�� �ִ� ���
	{
		Character->GetCharacterMovement()->JumpZVelocity = InitialJumpVelocity; // ���� �ӵ��� �ʱ� ���� �ӵ��� ����
	}
	MulticastJumpBuff(InitialJumpVelocity); // �ٸ� Ŭ���̾�Ʈ�鿡�� ���� �ӵ� �ʱ�ȭ �˸�
}

// ������Ʈ�� �ֿ� ƽ �Լ�
// DeltaTime: ���� ������ ������ �ð�
// TickType: ƽ ����
// ThisTickFunction: �� ƽ �Լ�
void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); // �θ� Ŭ������ TickComponent ȣ��

	HealRampUp(DeltaTime); // ü�� ȸ�� �ӵ� ���
	ShieldRampUp(DeltaTime); // ���� ���� �ӵ� ���
}

