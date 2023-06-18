// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// AProjectile Ŭ���� �����Դϴ�. �� Ŭ������ �߻�ü�� ��Ÿ���ϴ�.
UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// �������Դϴ�.
	AProjectile();

	// Tick �޼��� �������̵��Դϴ�. �� �����Ӹ��� ȣ��˴ϴ�.
	// DeltaTime: ���� �����Ӱ��� �ð� ����
	virtual void Tick(float DeltaTime) override;

	// Destroyed �޼��� �������̵��Դϴ�. ���Ͱ� �ı��Ǿ��� �� ȣ��˴ϴ�.
	virtual void Destroyed() override;

	// ���� �� �����ε带 ����ϴ��� ����
	bool bUseServerSideRewind = false;

	// Ʈ���̽� ���� ��ġ
	FVector_NetQuantize TraceStart;

	// �ʱ� �ӵ�
	FVector_NetQuantize100 InitialVelocity;

	// �߻�ü�� �ʱ� �ӵ� (�⺻��: 15000)
	UPROPERTY(EditAnywhere)
		float InitialSpeed = 15000;

	// ���ط� (�⺻��: 20.f). �̴� �׷����̵�� ���Ͽ��� ����˴ϴ�.
	UPROPERTY(EditAnywhere)
		float Damage = 20.f;

	// ��弦 ���ط� (�⺻��: 40.f). �̴� �׷����̵�� ���Ͽ��� ������� �ʽ��ϴ�.
	UPROPERTY(EditAnywhere)
		float HeadShotDamage = 40.f;

protected:
	// BeginPlay �޼��� �������̵��Դϴ�. ���Ͱ� ���ӿ� ��ġ�Ǹ� ȣ��˴ϴ�.
	virtual void BeginPlay() override;

	// �ı� Ÿ�̸Ӹ� �����ϴ� �޼���
	void StartDestroyTimer();

	// �ı� Ÿ�̸Ӱ� ������ �� ȣ��Ǵ� �޼���
	void DestroyTimerFinished();

	// ���� ��ƼŬ �ý����� �����ϴ� �޼���
	void SpawnTrailSystem();

	// ���� ���ظ� �����ϴ� �޼���
	void ExplodeDamage();

	// ������Ʈ�� �ٸ� ���ͳ� ������Ʈ�� �浹���� �� ȣ��Ǵ� �޼���
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// �浹 �� �����Ǵ� ��ƼŬ
	UPROPERTY(EditAnywhere)
		class UParticleSystem* ImpactParticles;

	// �浹 �� ����Ǵ� ����
	UPROPERTY(EditAnywhere)
		class USoundCue* ImpactSound;

	// �߻�ü�� �浹 �ڽ�
	UPROPERTY(EditAnywhere)
		class UBoxComponent* CollisionBox;

	// ���� ��ƼŬ �ý���
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* TrailSystem;

	// ���� ��ƼŬ �ý��� ������Ʈ
	UPROPERTY()
		class UNiagaraComponent* TrailSystemComponent;

	// �߻�ü�� �������� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	// �߻�ü�� �޽�
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ProjectileMesh;

	// ���ظ� ������ ������ ���� ������ (�⺻��: 200.f)
	UPROPERTY(EditAnywhere)
		float DamageInnerRadius = 200.f;

	// ���ظ� ������ ������ �ܺ� ������ (�⺻��: 500.f)
	UPROPERTY(EditAnywhere)
		float DamageOuterRadius = 500.f;

private:
	// Ʈ���̼� ��ƼŬ �ý����Դϴ�. �߻�ü�� ������ �ð������� ��Ÿ���� ������ �մϴ�.
	UPROPERTY(EditAnywhere)
		UParticleSystem* Tracer;

	// Ʈ���̼� ��ƼŬ �ý��� ������Ʈ�Դϴ�. �̸� ���� Ʈ���̼� ��ƼŬ �ý����� ��ġ, ȸ��, ������ ���� ������ �� �ֽ��ϴ�.
	UPROPERTY()
		class UParticleSystemComponent* TracerComponent;

	// �߻�ü�� �ı��� ���� Ÿ�̸��Դϴ�. ������ �ð��� ������ �߻�ü�� �ı��˴ϴ�.
	FTimerHandle DestroyTimer;

	// �߻�ü�� �ı��Ǳ������ �ð��Դϴ�. ������ ���Դϴ�. �⺻���� 3���Դϴ�.
	UPROPERTY(EditAnywhere)
		float DestroyTime = 3.f;

public:	
};
