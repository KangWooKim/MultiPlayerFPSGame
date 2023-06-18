// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ���� ������Ʈ ������
	UBuffComponent();

	// ABlasterCharacter Ŭ������ UBuffComponent�� private �� protected ����� ������ �� �ֵ��� ģ�� Ŭ������ ����
	friend class ABlasterCharacter;

	// ü�� ȸ�� �޼��� (HealAmount: ȸ����, HealingTime: ȸ���� �ɸ��� �ð�)
	void Heal(float HealAmount, float HealingTime);

	// ���� ä��� �޼��� (ShieldAmount: ä�� ���з�, ReplenishTime: ä��µ� �ɸ��� �ð�)
	void ReplenishShield(float ShieldAmount, float ReplenishTime);

	// �ӵ� ���� �޼��� (BuffBaseSpeed: ������ ���� ������ �⺻ �ӵ�, BuffCrouchSpeed: ������ ���� ������ ���� ������ �ӵ�, BuffTime: ���� ���� �ð�)
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);

	// ���� ���� �޼��� (BuffJumpVelocity: ������ ���� ������ ���� �ӵ�, BuffTime: ���� ���� �ð�)
	void BuffJump(float BuffJumpVelocity, float BuffTime);

	// �ʱ� �ӵ� ���� �޼��� (BaseSpeed: �⺻ �ӵ�, CrouchSpeed: ���� ������ �ӵ�)
	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);

	// �ʱ� ���� �ӵ� ���� �޼��� (Velocity: �ʱ� ���� �ӵ�)
	void SetInitialJumpVelocity(float Velocity);

protected:
	// ���� �� ȣ��Ǵ� �޼���
	virtual void BeginPlay() override;

	// ü�� ȸ�� �ӵ� ��� �޼��� (DeltaTime: ���� ������ ������ �ð�)
	void HealRampUp(float DeltaTime);

	// ���� ���� �ӵ� ��� �޼��� (DeltaTime: ���� ������ ������ �ð�)
	void ShieldRampUp(float DeltaTime);

private:
	// ĳ���� �Ӽ�
	UPROPERTY()
		class ABlasterCharacter* Character;

	// ü�� ȸ�� ���� ���� �Ӽ�
	bool bHealing = false; // ü�� ȸ�� ������ ����
	float HealingRate = 0.f; // ü�� ȸ�� �ӵ�
	float AmountToHeal = 0.f; // ȸ���� ü�·�

	// ���� ���� ���� ���� �Ӽ�
	bool bReplenishingShield = false; // ���� ���� ������ ����
	float ShieldReplenishRate = 0.f; // ���� ���� �ӵ�
	float ShieldReplenishAmount = 0.f; // ������ ���з�

	// �ӵ� ���� ���� �Ӽ�
	FTimerHandle SpeedBuffTimer; // �ӵ� ���� Ÿ�̸�
	void ResetSpeeds(); // �ӵ� �ʱ�ȭ �޼���
	float InitialBaseSpeed; // �ʱ� �⺻ �ӵ�
	float InitialCrouchSpeed; // �ʱ� ���� ������ �ӵ�

	// ��Ʈ��ũ ���� ��� Ŭ���̾�Ʈ���� �ӵ� ���� �˸� �޼���
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	// ���� ���� ���� �Ӽ�
	FTimerHandle JumpBuffTimer; // ���� ���� Ÿ�̸�
	void ResetJump(); // ���� �ʱ�ȭ �޼���
	float InitialJumpVelocity; // �ʱ� ���� �ӵ�

	// ��Ʈ��ũ ���� ��� Ŭ���̾�Ʈ���� ���� ���� �˸� �޼���
	UFUNCTION(NetMulticast, Reliable)
		void MulticastJumpBuff(float JumpVelocity);

public:
	// ������Ʈ�� �ֿ� ƽ �Լ� (DeltaTime: ���� ������ ������ �ð�, TickType: ƽ ����, ThisTickFunction: �� ƽ �Լ�)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
