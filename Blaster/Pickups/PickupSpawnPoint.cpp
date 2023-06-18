// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawnPoint.h"
#include "Pickup.h"

APickupSpawnPoint::APickupSpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false; // 이 액터의 틱을 비활성화합니다.
    bReplicates = true; // 네트워크에서 복제를 허용합니다.
}

void APickupSpawnPoint::BeginPlay()
{
    Super::BeginPlay(); // 부모 클래스의 BeginPlay를 호출합니다.
    StartSpawnPickupTimer((AActor*)nullptr); // 픽업 스폰 타이머를 시작합니다.
}

void APickupSpawnPoint::SpawnPickup()
{
    int32 NumPickupClasses = PickupClasses.Num(); // 가능한 픽업 클래스의 수를 얻습니다.
    if (NumPickupClasses > 0)
    {
        int32 Selection = FMath::RandRange(0, NumPickupClasses - 1); // 무작위 픽업 클래스를 선택합니다.
        SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform()); // 선택한 클래스의 픽업을 이 액터의 위치와 회전으로 스폰합니다.

        if (HasAuthority() && SpawnedPickup)
        {
            SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnPoint::StartSpawnPickupTimer); // 픽업이 파괴될 때 StartSpawnPickupTimer를 호출하도록 설정합니다.
        }
    }
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
    if (HasAuthority())
    {
        SpawnPickup(); // 픽업 스폰 타이머가 끝나면 픽업을 스폰합니다.
    }
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
    const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax); // 최소와 최대 사이에서 무작위 스폰 시간을 선택합니다.
    GetWorldTimerManager().SetTimer(
        spawnPickupTimer,
        this,
        &APickupSpawnPoint::SpawnPickupTimerFinished,
        SpawnTime // 스폰 시간 후에 SpawnPickupTimerFinished를 호출하도록 타이머를 설정합니다.
    );
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // 부모 클래스의 Tick을 호출합니다.

    // 추가적인 Tick 동작이 필요하면 이곳에 코드를 작성하세요.
}


