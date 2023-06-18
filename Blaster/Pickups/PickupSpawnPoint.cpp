// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawnPoint.h"
#include "Pickup.h"

APickupSpawnPoint::APickupSpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false; // �� ������ ƽ�� ��Ȱ��ȭ�մϴ�.
    bReplicates = true; // ��Ʈ��ũ���� ������ ����մϴ�.
}

void APickupSpawnPoint::BeginPlay()
{
    Super::BeginPlay(); // �θ� Ŭ������ BeginPlay�� ȣ���մϴ�.
    StartSpawnPickupTimer((AActor*)nullptr); // �Ⱦ� ���� Ÿ�̸Ӹ� �����մϴ�.
}

void APickupSpawnPoint::SpawnPickup()
{
    int32 NumPickupClasses = PickupClasses.Num(); // ������ �Ⱦ� Ŭ������ ���� ����ϴ�.
    if (NumPickupClasses > 0)
    {
        int32 Selection = FMath::RandRange(0, NumPickupClasses - 1); // ������ �Ⱦ� Ŭ������ �����մϴ�.
        SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform()); // ������ Ŭ������ �Ⱦ��� �� ������ ��ġ�� ȸ������ �����մϴ�.

        if (HasAuthority() && SpawnedPickup)
        {
            SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnPoint::StartSpawnPickupTimer); // �Ⱦ��� �ı��� �� StartSpawnPickupTimer�� ȣ���ϵ��� �����մϴ�.
        }
    }
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
    if (HasAuthority())
    {
        SpawnPickup(); // �Ⱦ� ���� Ÿ�̸Ӱ� ������ �Ⱦ��� �����մϴ�.
    }
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
    const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax); // �ּҿ� �ִ� ���̿��� ������ ���� �ð��� �����մϴ�.
    GetWorldTimerManager().SetTimer(
        spawnPickupTimer,
        this,
        &APickupSpawnPoint::SpawnPickupTimerFinished,
        SpawnTime // ���� �ð� �Ŀ� SpawnPickupTimerFinished�� ȣ���ϵ��� Ÿ�̸Ӹ� �����մϴ�.
    );
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // �θ� Ŭ������ Tick�� ȣ���մϴ�.

    // �߰����� Tick ������ �ʿ��ϸ� �̰��� �ڵ带 �ۼ��ϼ���.
}


