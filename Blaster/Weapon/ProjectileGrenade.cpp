// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// AProjectileGrenade Ŭ���� ������
AProjectileGrenade::AProjectileGrenade()
{
    // �׷����̵� �޽� ������Ʈ ����
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));

    // ��Ʈ ������Ʈ�� �׷����̵� �޽ø� ÷��
    ProjectileMesh->SetupAttachment(RootComponent);

    // �׷����̵� �޽��� �浹�� ��Ȱ��ȭ
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // ������Ʈ �̵� ������Ʈ ����
    ProjectileMovementComponent = CreateDefaultSubobject<UPrjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

    // �̵� ������Ʈ�� ȸ���� �ӵ��� �������� ����
    ProjectileMovementComponent->bRotationFollowsVelocity = true;

    // �̵� ������Ʈ ���� ���
    ProjectileMovementComponent->SetIsReplicated(true);

    // �̵� ������Ʈ�� �ٿ ���
    ProjectileMovementComponent->bShouldBounce = true;
}

// BeginPlay �Լ� �������̵�
void AProjectileGrenade::BeginPlay()
{
    // AActor Ŭ������ BeginPlay ȣ��
    AActor::BeginPlay();

    // Ʈ���� �ý��� ����
    SpawnTrailSystem();

    // �ı� Ÿ�̸� ����
    StartDestroyTimer();

    // �̵� ������Ʈ�� �ٿ �̺�Ʈ�� �Լ� ����
    ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileGrenade::OnBounce);
}

// �׷����̵��� �ٿ �̺�Ʈ �ڵ鷯
void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    // �ٿ ���尡 ���� ��� ���
    if (BounceSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            BounceSound,
            GetActorLocation()
        );
    }
}

// �׷����̵� �ı��� ȣ��Ǵ� �Լ�
void AProjectileGrenade::Destroyed()
{
    // ���� ������ ����
    ExplodeDamage();

    // AActor Ŭ������ Destroyed ȣ��
    Super::Destroyed();
}
