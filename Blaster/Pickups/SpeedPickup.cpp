// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

// ���ǵ� �Ⱦ��� �������� ���۵Ǿ��� �� ȣ��Ǵ� �޼���
void ASpeedPickup::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent, // �������� �⺻ ������Ʈ
    AActor* OtherActor, // �������� �ٸ� ����
    UPrimitiveComponent* OtherComp, // �������� �ٸ� ������Ʈ
    int32 OtherBodyIndex, // �������� �ٸ� ������Ʈ�� �ٵ� �ε���
    bool bFromSweep, // �������κ��� �������� ���۵Ǿ����� ����
    const FHitResult& SweepResult // ������ ���
) {
    // �θ� Ŭ������ OnSphereOverlap �޼��� ȣ��
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    // OtherActor�� ABlasterCharacter�� ĳ����
    ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
    // ĳ���ÿ� �����ϸ�
    if (BlasterCharacter)
    {
        // BlasterCharacter�� BuffComponent�� ����
        UBuffComponent* Buff = BlasterCharacter->GetBuff();
        // Buff�� ��ȿ�ϸ�
        if (Buff)
        {
            // Buff�� �ӵ��� ������Ŵ
            Buff->BuffSpeed(BaseSpeedBuff, CrouchSpeedBuff, SpeedBuffTime);
        }
    }

    // ���ǵ� �Ⱦ��� �ı�
    Destroy();
}
