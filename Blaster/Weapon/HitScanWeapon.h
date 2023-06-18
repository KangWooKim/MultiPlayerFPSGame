// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
 // ��Ʈ��ĵ ���� Ŭ����
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
    GENERATED_BODY()

public:
    // ���� �߻� �Լ��� ������ �մϴ�.
    // HitTarget: �߻� ��� ��ġ
    virtual void Fire(const FVector& HitTarget) override;

protected:
    // ��Ʈ ������ �����ϴ� �Լ�
    // TraceStart: ���� ���� ��ġ
    // HitTarget: �߻� ��� ��ġ
    // OutHit: ���� ���
    void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

    // ��Ʈ �� ��ƼŬ ȿ��
    UPROPERTY(EditAnywhere)
        class UParticleSystem* ImpactParticles;

    // ��Ʈ �� ����
    UPROPERTY(EditAnywhere)
        USoundCue* HitSound;

private:
    // ���� �� ��ƼŬ ȿ��
    UPROPERTY(EditAnywhere)
        UParticleSystem* BeamParticles;

    // �߻籸 ��ƼŬ ȿ��
    UPROPERTY(EditAnywhere)
        UParticleSystem* MuzzleFlash;

    // �߻� ����
    UPROPERTY(EditAnywhere)
        USoundCue* FireSound;
};
