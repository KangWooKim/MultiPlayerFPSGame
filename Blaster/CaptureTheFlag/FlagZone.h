// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blaster/BlasterTypes/Team.h"
#include "FlagZone.generated.h"

UCLASS()
class BLASTER_API AFlagZone : public AActor
{
	GENERATED_BODY()

public:
	// AFlagZone ������
	AFlagZone();

	// �� ���� ����. �����Ϳ��� ���� ����
	UPROPERTY(EditAnywhere)
		ETeam Team;

protected:
	// ������ ���۵� �� ȣ��Ǵ� �޼���
	virtual void BeginPlay() override;

	// ZoneSphere ������Ʈ�� �ٸ� ������Ʈ�� �������Ǹ� ȣ��Ǵ� �޼���
	// @param OverlappedComponent: ������ �̺�Ʈ�� �߻��� ������Ʈ
	// @param OtherActor: �������� �ٸ� ����
	// @param OtherComp: �������� �ٸ� ������Ʈ
	// @param OtherBodyIndex: �ٸ� ������Ʈ�� �ٵ� �ε���
	// @param bFromSweep: ���� ������� �������� �߻��ߴ����� ����
	// @param SweepResult: ������ ����� �����ϰ� �ִ� ����ü
	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

private:

	// �÷��� ���� ��Ÿ���� ���Ǿ� ������Ʈ. �����Ϳ��� ���� ����
	UPROPERTY(EditAnywhere)
		class USphereComponent* ZoneSphere;

public:

};
