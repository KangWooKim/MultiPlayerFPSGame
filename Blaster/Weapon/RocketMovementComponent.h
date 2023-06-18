// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RocketMovementComponent.generated.h"

/**
 * 
 */
 // ��� ���Ͽ��� ����� ���� Ŭ������ URocketMovementComponent�� �����ϰ� �⺻ Ŭ������ UProjectileMovementComponent���� ��ӹ޽��ϴ�.

UCLASS()
class BLASTER_API URocketMovementComponent : public UProjectileMovementComponent
{
    GENERATED_BODY()

protected:
    // HandleBlockingHit �޼��� �������̵�: ����(Blocking) ��Ʈ�� ó��
    // @param Hit: ����(Blocking) ��Ʈ ����� ���� ������ �����ϴ� FHitResult ����
    // @param TimeTick: �ð� ƽ(�̵��� ����Ǵ� ������ �ð� ����)�� ��Ÿ���� float ����
    // @param MoveDelta: �̵����� ��Ÿ���� FVector ����
    // @param SubTickTimeRemaining: ���� ƽ�� �����ִ� �ð��� ��Ÿ���� float ������ ���� ����
    // @return: EHandleBlockingHitResult �������� ��ȯ�ϸ�, ���� ���� �ܰ踦 �����϶�� ��ȣ�� ����
    virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;

    // HandleImpact �޼��� �������̵�: �浹�� ���� ó��
    // @param Hit: �浹 ����� ���� ������ �����ϴ� FHitResult ����
    // @param TimeSlice: �̵��� �ð� ������ ��Ÿ���� float ����, �⺻���� 0
    // @param MoveDelta: �̵����� ��Ÿ���� FVector ����, �⺻���� ���� ����
    virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;
};
