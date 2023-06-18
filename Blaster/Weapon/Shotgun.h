// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
U// AShotgun Ŭ���� ����
UCLASS()
class BLASTER_API AShotgun : public AHitScanWeapon
{
    GENERATED_BODY()

public:
    // ���� �߻� �޼���. �� �޼���� ���ǿ��� �߻�� �縴���� ��ǥ���ϴ� ��ġ���� �Ű������� �޽��ϴ�.
    // HitTargets: ���ǿ��� �߻�� �縴���� ��ǥ���ϴ� ��ġ���� ���� �迭�Դϴ�.
    virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);

    // ���� Ʈ���̽� �� ��ġ ��� �޼���. �� �޼���� ������ ��ǥ���ϴ� ��ġ��, ���ǿ��� �߻�� �縴���� ��ǥ���ϴ� ��ġ���� �Ű������� �޽��ϴ�.
    // HitTarget: ������ ��ǥ�� �ϴ� ��ġ�Դϴ�.
    // HitTargets: ���ǿ��� �߻�� �縴���� ��ǥ���ϴ� ��ġ���� ���� �迭�Դϴ�. �� �޼��忡���� �� �迭�� �縴���� ��ǥ�� �ϴ� ��ġ���� �߰��մϴ�.
    void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets);

private:
    // ���� �縴 ����. �� ������ �����Ϳ��� ���� �����մϴ�.
    UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
        uint32 NumberOfPellets = 10;
};
