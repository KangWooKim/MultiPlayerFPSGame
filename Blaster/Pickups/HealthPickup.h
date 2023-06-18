// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:
	// �⺻ �������Դϴ�. �� Ŭ������ ��ü�� ��Ʈ��ũ���� ���������ϵ��� �����մϴ�.
	AHealthPickup();

protected:
	// �� ��ü�� �ٸ� ��ü�� ��ġ�� ȣ��Ǵ� �޼����Դϴ�.
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent, // �� ��ü�� ��ģ ������Ʈ
		AActor* OtherActor, // �� ��ü�� ��ģ �ٸ� ����
		UPrimitiveComponent* OtherComp, // �� ��ü�� ��ģ �ٸ� ������Ʈ
		int32 OtherBodyIndex, // ��ģ �ٸ� ������Ʈ�� ��ü �ε���
		bool bFromSweep, // �� ��ħ�� �������κ��� �߻��� �������� ��Ÿ��
		const FHitResult& SweepResult // ���� ���� �߻��� ù ��° �浹�� ���� ���
	);

private:
	// �÷��̾��� ü���� �󸶳� ȸ���� �� �����ϴ� �����Դϴ�. ���� ����(EditAnywhere)�մϴ�.
	UPROPERTY(EditAnywhere)
		float HealAmount = 100.f;

	// �÷��̾ ü���� �󸶳� �������� ȸ������ �� �����ϴ� �����Դϴ�. ���� ����(EditAnywhere)�մϴ�.
	UPROPERTY(EditAnywhere)
		float HealingTime = 5.f;
};
