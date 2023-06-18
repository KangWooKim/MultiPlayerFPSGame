// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ShieldPickup.generated.h"

/**
 * 
 */
UCLASS()
// BLASTER_API�� ���� DLL �Ǵ� ���� ���̺귯���� ������ ���� ���
// APickup Ŭ������ ��ӹ޴� AShieldPickup Ŭ����
class BLASTER_API AShieldPickup : public APickup
{
    // Unreal Engine�� Ŭ���� ���� ��ũ��
    GENERATED_BODY()

protected:
    // �������� �߻����� �� ȣ��Ǵ� �Լ�. AShieldPickup Ŭ���������� �� �Լ��� �������մϴ�.
    virtual void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent,   // �������� ������Ʈ
        AActor* OtherActor,                         // �������� �ٸ� ����
        UPrimitiveComponent* OtherComp,             // �������� �ٸ� ������Ʈ
        int32 OtherBodyIndex,                       // �������� �ٵ��� �ε���
        bool bFromSweep,                            // �������� ���� �߻��ߴ��� ����
        const FHitResult& SweepResult               // ���� ���
    ) override;

private:
    // ���и� �����ϴ� ���� �����ϴ� ����. EditAnywhere�� ���� �����Ϳ��� ���� ����
    UPROPERTY(EditAnywhere)
    float ShieldReplenishAmount = 100.f;

    // ���и� �����ϴ� �� �ɸ��� �ð��� �����ϴ� ����. EditAnywhere�� ���� �����Ϳ��� ���� ����
    UPROPERTY(EditAnywhere)
    float ShieldReplenishTime = 5.f;
};

