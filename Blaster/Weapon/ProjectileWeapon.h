// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
 // Unreal�� AProjectileWeapon Ŭ���� ����
UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
    // Unreal ������ ���� ������ ����
    GENERATED_BODY()

public:
    // Fire �޼��带 �������̵�. �� �޼���� �߻縦 ó����.
    // @param HitTarget: �߻�ü�� �����ؾ� �ϴ� ��ġ
    virtual void Fire(const FVector& HitTarget) override;

private:
    // ProjectileClass ����. �� ������ �߻�� ������Ÿ���� Ŭ������ ����.
    UPROPERTY(EditAnywhere)
        TSubclassOf<class AProjectile> ProjectileClass;

    // ServerSideRewindProjectileClass ����. �� ������ ���� ���̵� �����ε带 ����� ��� �߻�� ������Ÿ���� Ŭ������ ����.
    UPROPERTY(EditAnywhere)
        TSubclassOf<AProjectile> ServerSideRewindProjectileClass;
};
