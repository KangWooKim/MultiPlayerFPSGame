// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileGrenade.generated.h"

/**
 * 
 */
 // UCLASS ��ũ�θ� ����� Unreal Engine�� ������Ʈ �ý��ۿ� Ŭ������ ����
UCLASS()
// AProjectileGrenade Ŭ������ AProjectile Ŭ������ ��ӹ޽��ϴ�.
class BLASTER_API AProjectileGrenade : public AProjectile
{
    // Ŭ������ �����ϰ� �����ϴ� �� �ʿ��� �ڵ带 �����ϴ� ��ũ��
    GENERATED_BODY()

        // AProjectileGrenade Ŭ������ public ������
public:
    // AProjectileGrenade Ŭ������ �⺻ ������
    AProjectileGrenade();

    // �� Ŭ������ �ı��� �� ȣ��Ǵ� �Լ��� �������̵�
    virtual void Destroyed() override;

    // AProjectileGrenade Ŭ������ protected ������
protected:
    // �÷��̰� ���۵� �� ȣ��Ǵ� �Լ��� �������̵�
    virtual void BeginPlay() override;

    // �׷����̵尡 �ٿ�� �� ȣ��Ǵ� �Լ�
    // - ImpactResult : �浹 ����� ���� �� ����
    // - ImpactVelocity : �浹 �� �׷����̵��� �ӵ�
    UFUNCTION()
        void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

    // AProjectileGrenade Ŭ������ private ������
private:
    // �׷����̵尡 �ٿ�� �� ����Ǵ� ���带 ��Ÿ���� �Ӽ�
    // ������ ��𼭳� ���� ����
    UPROPERTY(EditAnywhere)
        USoundCue* BounceSound;
};

