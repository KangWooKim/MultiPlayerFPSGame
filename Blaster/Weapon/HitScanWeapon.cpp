// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "WeaponTypes.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"

// źȯ�� �߻��ϴ� �Լ�
void AHitScanWeapon::Fire(const FVector& HitTarget)
{
    // �θ� Ŭ������ Fire �Լ��� ȣ���մϴ�.
    Super::Fire(HitTarget);

    // ������ Pawn�� �����ɴϴ�.
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    // ������ Pawn�� ������ �Լ��� �����մϴ�.
    if (OwnerPawn == nullptr) return;

    // ������ Pawn�� ��Ʈ�ѷ��� �����ɴϴ�.
    AController* InstigatorController = OwnerPawn->GetController();

    // ���� �޽����� "MuzzleFlash" ������ �����ɴϴ�.
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
    if (MuzzleFlashSocket)
    {
        // ������ ��ȯ ���� �����ɴϴ�.
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
        FVector Start = SocketTransform.GetLocation();

        FHitResult FireHit;
        // ���� �߻� ��Ʈ ����� �����ɴϴ�.
        WeaponTraceHit(Start, HitTarget, FireHit);

        // ��Ʈ ����κ��� ������ ĳ���͸� �����ɴϴ�.
        ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
        if (BlasterCharacter && InstigatorController)
        {
            // �������� ������ ������ Ȯ���մϴ�.
            bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
            if (HasAuthority() && bCauseAuthDamage)
            {
                // �Ӹ��� �¾Ҵ��� ���ο� ���� ������ ���� �����մϴ�.
                const float DamageToCause = FireHit.BoneName.ToString() == FString("head") ? HeadShotDamage : Damage;

                // �������� �����մϴ�.
                UGameplayStatics::ApplyDamage(
                    BlasterCharacter,
                    DamageToCause,
                    InstigatorController,
                    this,
                    UDamageType::StaticClass()
                );
            }
            // ���� ���̵� �����ε带 ����ϴ� ���
            if (!HasAuthority() && bUseServerSideRewind)
            {
                BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
                BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
                if (BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
                {
                    // ���� ��û�� ������ �����մϴ�.
                    BlasterOwnerCharacter->GetLagCompensation()->ServerScoreRequest(
                        BlasterCharacter,
                        Start,
                        HitTarget,
                        BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime
                    );
                }
            }
        }
        // ��ƼŬ ����Ʈ�� �����մϴ�.
        if (ImpactParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                ImpactParticles,
                FireHit.ImpactPoint,
                FireHit.ImpactNormal.Rotation()
            );
        }
        // ��Ʈ ���带 ����մϴ�.
        if (HitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                HitSound,
                FireHit.ImpactPoint
            );
        }

        // ���� �÷��� ����Ʈ�� �����մϴ�.
        if (MuzzleFlash)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                MuzzleFlash,
                SocketTransform
            );
        }
        // �߻� ���带 ����մϴ�.
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                FireSound,
                GetActorLocation()
            );
        }
    }
}

// ���⿡�� ��Ʈ ����� �����ϴ� �Լ�
void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
    // ���� ���带 �����ɴϴ�.
    UWorld* World = GetWorld();
    if (World)
    {
        // ��Ʈ Ÿ�� �������� ���� ������ ����մϴ�.
        FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;

        // �� ������ �����Ͽ� ��Ʈ ����� �����ɴϴ�.
        World->LineTraceSingleByChannel(
            OutHit,
            TraceStart,
            End,
            ECollisionChannel::ECC_Visibility
        );
        FVector BeamEnd = End;
        // ���ŷ ��Ʈ�� �߻��ϸ� �� ������ ��Ʈ ��ġ�� �����մϴ�.
        if (OutHit.bBlockingHit)
        {
            BeamEnd = OutHit.ImpactPoint;
        }
        // ���ŷ ��Ʈ�� ������ ��Ʈ ��ġ�� ���� �������� �����մϴ�.
        else
        {
            OutHit.ImpactPoint = End;
        }

        // �� ��ƼŬ�� �����մϴ�.
        if (BeamParticles)
        {
            UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
                World,
                BeamParticles,
                TraceStart,
                FRotator::ZeroRotator,
                true
            );
            // ������ �� ��ƼŬ�� Ÿ�� ��ġ�� �����մϴ�.
            if (Beam)
            {
                Beam->SetVectorParameter(FName("Target"), BeamEnd);
            }
        }
    }
}
