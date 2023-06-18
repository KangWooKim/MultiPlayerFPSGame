// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "RocketMovementComponent.h"

// AProjectileRocket Ŭ������ �����ڸ� ����
AProjectileRocket::AProjectileRocket()
{
    // "Rocket Mesh"��� �̸����� UStaticMeshComponent�� ����
    // �� �޽��� ������Ÿ���� ����� ����
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));

    // ������ �޽��� �� Ŭ������ RootComponent�� ����
    ProjectileMesh->SetupAttachment(RootComponent);

    // �޽��� �浹�� ��Ȱ��ȭ
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // "RocketMovementComponent"��� �̸����� URocketMovementComponent�� ����
    // �� ������Ʈ�� ������ �̵��� ����
    RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));

    // ������ ȸ���� �ӵ��� �������� ����
    RocketMovementComponent->bRotationFollowsVelocity = true;

    // �� ������Ʈ�� ��Ʈ��ũ���� �����ǵ��� ����
    RocketMovementComponent->SetIsReplicated(true);
}

// BeginPlay �޼��带 ����
// �� �޼���� ���Ͱ� ���ӿ��� Ȱ��ȭ�� �� ȣ��
void AProjectileRocket::BeginPlay()
{
    // �θ� Ŭ������ BeginPlay ȣ��
    Super::BeginPlay();

    // �� ���Ͱ� ���������� ����Ǵ� ������ �ƴ϶��
    if (!HasAuthority())
    {
        // CollisionBox�� �ٸ� ������Ʈ�� �浹�� �� OnHit �Լ��� ȣ��ǵ��� ����
        CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHit);
    }

    // ����Ʈ �ý��� ���� �Լ� ȣ��
    SpawnTrailSystem();

    // ProjectileLoop�� LoopingSoundAttenuation�� �����Ǿ� �ִٸ�
    if (ProjectileLoop && LoopingSoundAttenuation)
    {
        // Rocket�� LoopingSound�� �߰�
        ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(
            ProjectileLoop,
            GetRootComponent(),
            FName(),
            GetActorLocation(),
            EAttachLocation::KeepWorldPosition,
            false,
            1.0f,
            1.0f,
            0.0f,
            LoopingSoundAttenuation,
            (USoundConcurrency*)nullptr,
            false
        );
    }
}


// AProjectileRocket Ŭ������ OnHit �޼��带 ����
// �� �޼���� �� ������Ʈ�� �ٸ� ���Ϳ� �浹���� �� ȣ��
void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // ���� �浹�� ���Ͱ� �� ������Ÿ���� �����ڶ�� �ƹ� �۾��� ���� �ʰ� ��ȯ
    if (OtherActor == GetOwner())
    {
        return;
    }

    // �浹�� ���� ���� �������� ó��
    ExplodeDamage();

    // �ı� Ÿ�̸� ����
    StartDestroyTimer();

    // ImpactParticles�� �����Ǿ� �ִٸ�
    // �浹 ��ġ�� ���� �ý����� ����
    if (ImpactParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
    }

    // ImpactSound�� �����Ǿ� �ִٸ�
    // �浹 ��ġ���� ���带 ���
    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    }

    // ������Ÿ�� �޽��� �ִٸ�
    // �̸� ������ �ʰ� ����
    if (ProjectileMesh)
    {
        ProjectileMesh->SetVisibility(false);
    }

    // �浹 ���ڰ� �ִٸ�
    // �̸� ��Ȱ��ȭ
    if (CollisionBox)
    {
        CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // TrailSystemComponent�� ���� �ν��Ͻ��� �ִٸ�
    // �̸� ��Ȱ��ȭ
    if (TrailSystemComponent && TrailSystemComponent->GetSystemInstance())
    {
        TrailSystemComponent->GetSystemInstance()->Deactivate();
    }

    // ProjectileLoopComponent�� ��� ���̶��
    // �̸� ����
    if (ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
    {
        ProjectileLoopComponent->Stop();
    }
}

// AProjectileRocket Ŭ������ Destroyed �޼��带 ����
// �� �޼���� �� ������Ʈ�� �ı��� �� ȣ��
void AProjectileRocket::Destroyed()
{

}
