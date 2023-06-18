// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BLASTER_API APickup : public AActor
{
	GENERATED_BODY()

public:
	APickup(); // �������Դϴ�.
	virtual void Tick(float DeltaTime) override; // �����Ӹ��� ȣ��˴ϴ�. DeltaTime�� ���� �����Ӱ��� �ð� ���Դϴ�.
	virtual void Destroyed() override; // �� ���Ͱ� �ı��� �� ȣ��˴ϴ�.

protected:
	virtual void BeginPlay() override; // ������ ���۵Ǿ��� �� ȣ��˴ϴ�.

	// �� �Լ��� OverlapSphere�� �ٸ� ������Ʈ�� ��ġ�� ������ �� ȣ��˴ϴ�.
	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent, // ��ġ�� ������ ������Ʈ�Դϴ�.
			AActor* OtherActor, // ��ġ�� ������ �ٸ� �����Դϴ�.
			UPrimitiveComponent* OtherComp, // ��ġ�� ������ �ٸ� ������Ʈ�Դϴ�.
			int32 OtherBodyIndex, // ��ģ �ٸ� ������Ʈ�� �ٵ� �ε����Դϴ�.
			bool bFromSweep, // �� ��ħ�� �������κ��� �߻��ߴ����� �����Դϴ�.
			const FHitResult& SweepResult // ������ ����Դϴ�.
		);

	// ȸ�� �ӵ��� �����մϴ�. ������ �ʴ� ���Դϴ�.
	UPROPERTY(EditAnywhere)
		float BaseTurnRate = 45.f;

private:
	// �÷��̾ �Ⱦ��� ������ �� �ִ� ������ ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere)
		class USphereComponent* OverlapSphere;

	// �Ⱦ��� ������ �� ����Ǵ� �����Դϴ�.
	UPROPERTY(EditAnywhere)
		class USoundCue* PickupSound;

	// �Ⱦ��� �޽ø� ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickupMesh;

	// �Ⱦ��� ÷�ε� ȿ���� ��Ÿ���ϴ�.
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* PickupEffectComponent;

	// �Ⱦ��� ������ �� �����Ǵ� ȿ���Դϴ�.
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* PickupEffect;

	// OverlapSphere�� OnComponentBeginOverlap �̺�Ʈ�� ���ε��ϴ� Ÿ�̸Ӹ� �����մϴ�.
	FTimerHandle BindOverlapTimer;
	float BindOverlapTime = 0.25f;
	void BindOverlapTimerFinished(); // BindOverlapTimer�� ������ �� ȣ��˴ϴ�.

public:
};
