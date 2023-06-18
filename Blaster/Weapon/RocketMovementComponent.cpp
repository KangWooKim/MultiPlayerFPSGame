// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketMovementComponent.h"

// URocketMovementComponent Ŭ���� ���� �޼������ ����

// HandleBlockingHit �޼���: ����(Blocking) ��Ʈ�� ó��
// @param Hit: ����(Blocking) ��Ʈ ����� ���� ������ �����ϴ� FHitResult ����
// @param TimeTick: �ð� ƽ(�̵��� ����Ǵ� ������ �ð� ����)�� ��Ÿ���� float ����
// @param MoveDelta: �̵����� ��Ÿ���� FVector ����
// @param SubTickTimeRemaining: ���� ƽ�� �����ִ� �ð��� ��Ÿ���� float ������ ���� ����
// @return: EHandleBlockingHitResult �������� ��ȯ�ϸ�, ���� ���� �ܰ踦 �����϶�� ��ȣ�� ����
URocketMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
    Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
    return EHandleBlockingHitResult::AdvanceNextSubstep;
}

// HandleImpact �޼���: �浹�� ���� ó��
// @param Hit: �浹 ����� ���� ������ �����ϴ� FHitResult ����
// @param TimeSlice: �̵��� �ð� ������ ��Ÿ���� float ����
// @param MoveDelta: �̵����� ��Ÿ���� FVector ����
void URocketMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
    // ������ ���߾�� �� �˴ϴ�; ���� �׵��� CollisionBox�� �浹�� ������ ���� �����ؾ� �մϴ�.
}
