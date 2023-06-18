// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
 // UCLASS ��ũ�θ� ����� Unreal Engine�� ������Ʈ �ý��ۿ� Ŭ������ ����
UCLASS()
// AProjectileBullet Ŭ������ AProjectile Ŭ������ ��ӹ޽��ϴ�.
class BLASTER_API AProjectileBullet : public AProjectile
{
    // Ŭ������ �����ϰ� �����ϴ� �� �ʿ��� �ڵ带 �����ϴ� ��ũ��
    GENERATED_BODY()

        // AProjectileBullet Ŭ������ public ������
public:
    // AProjectileBullet Ŭ������ �⺻ ������
    AProjectileBullet();

    // �����Ϳ��� �Ӽ��� ����Ǿ��� �� ȣ��Ǵ� �Լ�
#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
#endif

    // AProjectileBullet Ŭ������ protected ������
protected:
    // ��ӹ��� Ŭ������ OnHit �޼ҵ带 �������̵�
    // �� �޼ҵ�� ����ü�� ��� �浹���� �� ȣ���
    // - HitComp : �浹�� ������Ʈ
    // - OtherActor : �浹�� �ٸ� ����
    // - OtherComp : �浹�� �ٸ� ������Ʈ
    // - NormalImpulse : �浹�� ���� ������ ���� ���
    // - Hit : �������� �浹 ����� ���� ����
    virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

    // ��ӹ��� Ŭ������ BeginPlay �޼ҵ带 �������̵�
    // �� �޼ҵ�� �÷��̰� ���۵� �� ȣ���
    virtual void BeginPlay() override;
};
