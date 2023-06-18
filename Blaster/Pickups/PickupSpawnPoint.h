// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class BLASTER_API APickupSpawnPoint : public AActor
{
    GENERATED_BODY()

public:
    APickupSpawnPoint(); // ������
    virtual void Tick(float DeltaTime) override; // ƽ �޼���. DeltaTime�� ���� ������ ������ �ð��Դϴ�.

protected:
    virtual void BeginPlay() override; // ������ ���۵� �� ȣ��˴ϴ�.

    UPROPERTY(EditAnywhere) // UE5 �����Ϳ��� ���� �����ϵ��� �մϴ�.
    TArray<TSubclassOf<class APickup>> PickupClasses; // ������ �Ⱦ� Ŭ�������� �����մϴ�.

    UPROPERTY() // �ش� ������ UE5 �ý��ۿ� ���� �����ǵ��� �մϴ�.
    APickup* SpawnedPickup; // ������ �Ⱦ��� �����մϴ�.

    void SpawnPickup(); // �Ⱦ��� �����ϴ� �޼����Դϴ�.
    void SpawnPickupTimerFinished(); // �Ⱦ� ���� Ÿ�̸Ӱ� �Ϸ�� �� ȣ��˴ϴ�.

    UFUNCTION() // UE5 �ý��ۿ��� �� �Լ��� �ν��ϵ��� �մϴ�.
    void StartSpawnPickupTimer(AActor* DestroyedActor); // �Ⱦ� ���� Ÿ�̸Ӹ� �����մϴ�. DestroyedActor�� �ı��� ���͸� �����մϴ�.

private:
    FTimerHandle SpawnPickupTimer; // �Ⱦ� ���� Ÿ�̸� �ڵ��� �����մϴ�.

    UPROPERTY(EditAnywhere) // UE5 �����Ϳ��� ���� �����ϵ��� �մϴ�.
    float SpawnPickupTimeMin; // �Ⱦ� ���� �ּ� �ð��� �����մϴ�.

    UPROPERTY(EditAnywhere) // UE5 �����Ϳ��� ���� �����ϵ��� �մϴ�.
    float SpawnPickupTimeMax; // �Ⱦ� ���� �ִ� �ð��� �����մϴ�.

public:
    // �߰����� public ������� �̰��� �߰��ϼ���.

};
