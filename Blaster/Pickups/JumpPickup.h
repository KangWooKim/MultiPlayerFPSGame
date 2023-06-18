// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "JumpPickup.generated.h"

/**
 * 
 */
UCLASS()
// �� Ŭ������ APickup Ŭ������ ��ӹ޾� ���� �Ⱦ� �������� �����մϴ�.
class BLASTER_API AJumpPickup : public APickup
{
	GENERATED_BODY()

protected:
    // �� �޼���� ����ü���� �������̵��ؾ� �ϸ�, ���Ǿ� ������Ʈ�� �ٸ� ���ͳ� ������Ʈ�� ������ �� ȣ��˴ϴ�.
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,  // �� ��ü�� ��ġ�� ������Ʈ
		AActor* OtherActor,  // �� ��ü�� ��ġ�� �ٸ� ����
		UPrimitiveComponent* OtherComp,  // �� ��ü�� ��ġ�� �ٸ� ������Ʈ
		int32 OtherBodyIndex,  // ��ġ�� �ٸ� ������Ʈ�� ��ü �ε���
		bool bFromSweep,  // �� ��ħ�� ���� ���� �߻��ߴ��� ����
		const FHitResult& SweepResult  // ���� ���� �߻��� ù �浹�� ���� ���
	);

private:
    // �� ������ ���� �Ⱦ� �������� �÷��̾ ȹ���ϸ� ����� ���� �ӵ��� �������� �����մϴ�.
	UPROPERTY(EditAnywhere)
	float JumpZVelocityBuff = 4000.f;

    // �� ������ ���� �ӵ� ���� ȿ���� ���ӵ� �ð��� �����մϴ�.
	UPROPERTY(EditAnywhere)
	float JumpBuffTime = 30.f;
};
