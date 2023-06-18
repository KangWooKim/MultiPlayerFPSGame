// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/CombatComponent.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, // �� ������Ʈ�� �浹�� �߻��� ������Ʈ
    AActor* OtherActor, // �� ������Ʈ�� �浹�� �ٸ� ����
    UPrimitiveComponent* OtherComp, // �浹�� �߻��� �ٸ� ������Ʈ
    int32 OtherBodyIndex, // �浹�� �ٸ� ������Ʈ�� ��ü �ε���
    bool bFromSweep, // �� �浹�� ����(sweep: ������Ʈ�� �����̸鼭 ��λ� �ִ� ��� ������Ʈ���� �浹�� �����ϴ� ���)���κ��� �߻��� �������� ��Ÿ��
    const FHitResult& SweepResult) // ���� ���� �߻��� ù ��° �浹�� ���� ���
{
    // �θ� Ŭ������ OnSphereOverlap �޼��带 ȣ���մϴ�.
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    // OtherActor�� ABlasterCharacter�� ĳ�����մϴ�.
    // ĳ���� ����� ��ȿ�� ��쿡�� �Ʒ� ������ �����մϴ�.
    ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
    if (BlasterCharacter)
    {
        // BlasterCharacter�� Combat ������Ʈ�� �����ɴϴ�.
        // Combat ������Ʈ�� ��ȿ�� ��쿡�� �Ʒ� ������ �����մϴ�.
        UCombatComponent* Combat = BlasterCharacter->GetCombat();
        if (Combat)
        {
            // Combat ������Ʈ�� ź���� �����ݴϴ�.
            Combat->PickupAmmo(WeaponType, AmmoAmount);
        }
    }
    // ź���� �ֿ� �Ŀ��� ź�� �Ⱦ� ��ü�� �ı��մϴ�.
    Destroy();
}
