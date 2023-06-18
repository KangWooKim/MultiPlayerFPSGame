// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
// "ACasing"�� ź�Ǹ� ��Ÿ���� Ŭ�����Դϴ�.
class BLASTER_API ACasing : public AActor
{
	GENERATED_BODY()

public:
	// �⺻ �������Դϴ�.
	ACasing();

protected:
	// ���� ���� �� ȣ��Ǵ� �Լ��Դϴ�.
	virtual void BeginPlay() override;

	// �浹 �� ȣ��Ǵ� �Լ��Դϴ�.
	// HitComp: �浹�� �߻��� �� ������Ʈ�Դϴ�.
	// OtherActor: �� ������Ʈ�� �浹�� �ٸ� �����Դϴ�.
	// OtherComp: �浹�� �ٸ� ������ ������Ʈ�Դϴ�.
	// NormalImpulse: �浹 �� �־��� ����Դϴ�.
	// Hit: �浹�� ���� �� �����Դϴ�.
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// ź���� �޽� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CasingMesh;

	// ź�ǰ� �ѿ��� ���� ���� �������Դϴ�.
	UPROPERTY(EditAnywhere)
		float ShellEjectionImpulse;

	// ź�ǰ� �浹 �� ����Ǵ� �����Դϴ�.
	UPROPERTY(EditAnywhere)
		class USoundCue* ShellSound;
};
