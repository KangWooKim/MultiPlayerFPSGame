// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

// AProjectileWeapon Ŭ������ Fire �޼��带 ����
// �� �޼���� ���� �߻縦 ó��
void AProjectileWeapon::Fire(const FVector& HitTarget)
{
    // �θ� Ŭ������ Fire �޼��带 ȣ��
    Super::Fire(HitTarget);

    // �����ڸ� APawn Ÿ������ ĳ����
    APawn* InstigatorPawn = Cast<APawn>(GetOwner());

    // ������ ���� �÷��� ������ ������
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

    // ���� ���带 ������
    UWorld* World = GetWorld();

    // ���� �÷��� ���ϰ� ���尡 ��� ��ȿ�� ��쿡�� ����
    if (MuzzleFlashSocket && World)
    {
        // ���� �÷��� ������ ��ġ�� ����
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        // ���� �÷��� ���� ��ġ���� ���� ��ġ������ ���͸� ����
        FVector ToTarget = HitTarget - SocketTransform.GetLocation();

        // ���� ������ �������� ȸ�� ���͸� ����
        FRotator TargetRotation = ToTarget.Rotation();

        // ���͸� �����ϱ� ���� �Ķ���͸� ����
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.Instigator = InstigatorPawn;

        // ������ ������Ÿ�� ���� ����
        AProjectile* SpawnedProjectile = nullptr;

        // ���� ���̵� �����ε�(SSR) ��� ���ο� ���� �б�
        // SSR�̶�, Ŭ���̾�Ʈ�� ���� ���� �����ð��� �����ϱ� ���� �Ѿ� �߻縦 �������� ó���ϴ� ���
        if (bUseServerSideRewind)
        {
            // ���������� ó��
            if (InstigatorPawn->HasAuthority()) // �������� ����
            {
                if (InstigatorPawn->IsLocallyControlled()) // ����, ȣ��Ʈ - ������ ������Ÿ�� ���
                {
                    SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
                    SpawnedProjectile->bUseServerSideRewind = false;
                    SpawnedProjectile->Damage = Damage;
                    SpawnedProjectile->HeadShotDamage = HeadShotDamage;
                }
                else // ����, ���� ������� ���� - �� ���� ������Ÿ�� ����, SSR ���
                {
                    SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
                    SpawnedProjectile->bUseServerSideRewind = true;
                }
            }
            else // Ŭ���̾�Ʈ���� SSR ���
            {
                if (InstigatorPawn->IsLocallyControlled()) // Ŭ���̾�Ʈ, ���� ���� - �� ���� ������Ÿ�� ����, SSR ���
                {
                    SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
                    SpawnedProjectile->bUseServerSideRewind = true;
                    SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
                    SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
                }
                else // Ŭ���̾�Ʈ, ���� ������� ���� - �� ���� ������Ÿ�� ����, SSR ��� �� ��
                {
                    SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
                    SpawnedProjectile->bUseServerSideRewind = false;
                }
            }
        }
        else // SSR�� ������� �ʴ� ����
        {
            if (InstigatorPawn->HasAuthority())
            {
                SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
                SpawnedProjectile->bUseServerSideRewind = false;
                SpawnedProjectile->Damage = Damage;
                SpawnedProjectile->HeadShotDamage = HeadShotDamage;
            }
        }
    }
}
