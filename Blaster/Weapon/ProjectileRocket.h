// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
#pragma once

UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
    GENERATED_BODY()

public:
    AProjectileRocket();

    // �ı��� �� ȣ��Ǵ� �޼����Դϴ�.
    virtual void Destroyed() override;

protected:
    // �浹 �� ȣ��Ǵ� �޼����Դϴ�.
    // HitComp: �浹�� ������ UPrimitiveComponent
    // OtherActor: �浹�� �ٸ� ����
    // OtherComp: �浹�� �ٸ� ������ UPrimitiveComponent
    // NormalImpulse: �浹 �� ������ �������� ���
    // Hit: �浹 ����� ���� ����
    virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

    // ���� ���� �� ȣ��Ǵ� �޼����Դϴ�.
    virtual void BeginPlay() override;

    // ������Ÿ�� ������ ���Ǵ� ���� ť
    UPROPERTY(EditAnywhere)
        USoundCue* ProjectileLoop;

    // ������Ÿ�� ������ ���Ǵ� ����� ������Ʈ
    UPROPERTY()
        UAudioComponent* ProjectileLoopComponent;

    // ���� ������ �Ҹ� ���� ����
    UPROPERTY(EditAnywhere)
        USoundAttenuation* LoopingSoundAttenuation;

    // ���� �̵� ������Ʈ
    UPROPERTY(VisibleAnywhere)
        class URocketMovementComponent* RocketMovementComponent;

private:
};
