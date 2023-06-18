// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Blaster.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// AProjectile Ŭ���� �������Դϴ�.
AProjectile::AProjectile()
{
    // �� ���Ͱ� tick�� ���� �� �ִ��� �����մϴ�.
    PrimaryActorTick.bCanEverTick = true;

    // ��Ʈ��ũ �󿡼� ������ �� �ִ��� �����մϴ�.
    bReplicates = true;

    // CollisionBox ������Ʈ�� �����ϰ� root�� �����մϴ�.
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    SetRootComponent(CollisionBox);

    // �浹 �����Դϴ�.
    CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECCollisionResponse::ECR_Block);
    CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECCollisionResponse::ECR_Block);
    CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECCollisionResponse::ECR_Block);
}

// BeginPlay �޼����Դϴ�.
void AProjectile::BeginPlay()
{
    // �θ��� BeginPlay�� ȣ���մϴ�.
    Super::BeginPlay();

    // Tracer�� �ִ� ��� TracerComponent�� ���Դϴ�.
    if (Tracer)
    {
        TracerComponent = UGameplayStatics::SpawnEmitterAttached(
            Tracer,
            CollisionBox,
            FName(),
            GetActorLocation(),
            GetActorRotation(),
            EAttachLocation::KeepWorldPosition
        );
    }

    // ������ �ִ� ��� OnHit �̺�Ʈ�� ����մϴ�.
    if (HasAuthority())
    {
        CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    }
}

// OnHit �޼����Դϴ�. ���𰡿� �ε����� �� ȣ��˴ϴ�.
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // �� ���͸� �ı��մϴ�.
    Destroy();
}

// SpawnTrailSystem �޼����Դϴ�. ���� �ý����� �����մϴ�.
void AProjectile::SpawnTrailSystem()
{
    if (TrailSystem)
    {
        TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            TrailSystem,
            GetRootComponent(),
            FName(),
            GetActorLocation(),
            GetActorRotation(),
            EAttachLocation::KeepWorldPosition,
            false
        );
    }
}


// AProjectile::ExplodeDamage �޼����Դϴ�. ���� ���ظ� �����մϴ�.
void AProjectile::ExplodeDamage()
{
    // �߻��� Pawn�� �����ɴϴ�.
    APawn* FiringPawn = GetInstigator();
    // ���� ������ �ְ�, �߻��� Pawn�� �ִ� ���
    if (FiringPawn && HasAuthority())
    {
        // �߻��� Pawn�� ��Ʈ�ѷ��� �����ɴϴ�.
        AController* FiringController = FiringPawn->GetController();
        if (FiringController)
        {
            // ���� ���� ���� ���ظ� �����մϴ�. 
            UGameplayStatics::ApplyRadialDamageWithFalloff(
                this, // ���� ���ؽ�Ʈ ������Ʈ
                Damage, // �⺻ ����
                10.f, // �ּ� ����
                GetActorLocation(), // �߽� ��ġ
                DamageInnerRadius, // ���� ������
                DamageOuterRadius, // �ܺ� ������
                1.f, // ���� ������
                UDamageType::StaticClass(), // ���� Ÿ�� Ŭ����
                TArray<AActor*>(), // ������ ���͵�
                this, // ���ظ� ���� ��ü
                FiringController // �߻��� ��Ʈ�ѷ�
            );
        }
    }
}

// Tick �޼����Դϴ�. 
void AProjectile::Tick(float DeltaTime)
{
    // �θ��� Tick �޼��带 ȣ���մϴ�.
    Super::Tick(DeltaTime);
}

// StartDestroyTimer �޼����Դϴ�. �ı� Ÿ�̸Ӹ� �����մϴ�.
void AProjectile::StartDestroyTimer()
{
    // Ÿ�̸� �Ŵ����� ���� Ÿ�̸Ӹ� �����մϴ�.
    GetWorldTimerManager().SetTimer(
        DestroyTimer,
        this,
        &AProjectile::DestroyTimerFinished,
        DestroyTime
    );
}

// DestroyTimerFinished �޼����Դϴ�. �ı� Ÿ�̸Ӱ� ������ �� ȣ��˴ϴ�.
void AProjectile::DestroyTimerFinished()
{
    // �� ���͸� �ı��մϴ�.
    Destroy();
}

// Destroyed �޼����Դϴ�. ���Ͱ� �ı��Ǿ��� �� ȣ��˴ϴ�.
void AProjectile::Destroyed()
{
    // �θ��� Destroyed �޼��带 ȣ���մϴ�.
    Super::Destroyed();

    // �浹 ��ƼŬ�� �ִ� ���, ��ƼŬ�� �����մϴ�.
    if (ImpactParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
    }
    // �浹 ���尡 �ִ� ���, ���带 ����մϴ�.
    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    }
}

