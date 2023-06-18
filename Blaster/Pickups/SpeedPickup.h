// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "SpeedPickup.generated.h"

/**
 * 
 */
UCLASS()
// ASpeedPickup Ŭ���� ����
class BLASTER_API ASpeedPickup : public APickup
{
    GENERATED_BODY()

protected:
    // �������Ǵ� ��ü�� ���� ó���� ����ϴ� �Լ�
    virtual void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent, // �������� �߻��� �⺻ ������Ʈ
        AActor* OtherActor, // �������� �߻��� �ٸ� ����
        UPrimitiveComponent* OtherComp, // �������� �߻��� �ٸ� ������Ʈ
        int32 OtherBodyIndex, // �������� �߻��� �ٸ� ������Ʈ�� �ٵ� �ε���
        bool bFromSweep, // �������κ����� ���������� �ƴ����� ��Ÿ��
        const FHitResult& SweepResult // ������ ���
    );

private:
    // �����Ϳ��� ���� ������ �⺻ �ӵ� ����
    UPROPERTY(EditAnywhere)
    float BaseSpeedBuff = 1600.f;

    // �����Ϳ��� ���� ������ ���� ������ �ӵ� ����
    UPROPERTY(EditAnywhere)
    float CrouchSpeedBuff = 850.f;

    // �����Ϳ��� ���� ������ �ӵ� ���� ���� �ð�
    UPROPERTY(EditAnywhere)
    float SpeedBuffTime = 30.f;
};
