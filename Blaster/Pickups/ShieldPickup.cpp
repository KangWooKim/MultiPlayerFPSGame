// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

// AShieldPickup Ŭ������ OnSphereOverlap �Լ�
void AShieldPickup::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent,   // ������ �̺�Ʈ�� �߻��� ������Ʈ
    AActor* OtherActor,                         // ������ �̺�Ʈ�� �߻���Ų �ٸ� ����
    UPrimitiveComponent* OtherComp,             // ������ �̺�Ʈ�� �߻���Ų �ٸ� ������Ʈ
    int32 OtherBodyIndex,                       // �������� �߻��� �ٸ� ������Ʈ�� �ٵ� �ε���
    bool bFromSweep,                            // ���� ����κ����� ���������� ����
    const FHitResult& SweepResult               // ������ �߻����� ���� ��� ����
)
{
    // �θ� Ŭ������ ������ ó�� �Լ� ȣ��
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    // �������� �߻��� ���Ͱ� ABlasterCharacter �ν��Ͻ����� Ȯ��
    ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
    if (BlasterCharacter)
    {
        // ABlasterCharacter �ν��Ͻ����� UBuffComponent�� ������
        UBuffComponent* Buff = BlasterCharacter->GetBuff();
        if (Buff)
        {
            // UBuffComponent�� ���и� ����
            Buff->ReplenishShield(ShieldReplenishAmount, ShieldReplenishTime);
        }
    }

    // ������ ó���� ������ �ڽ��� �ı�
    Destroy();
}
