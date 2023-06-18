// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

void AJumpPickup::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComponent,  // �� ��ü�� ��ġ�� ������Ʈ
    AActor* OtherActor,  // �� ��ü�� ��ġ�� �ٸ� ����
    UPrimitiveComponent* OtherComp,  // �� ��ü�� ��ġ�� �ٸ� ������Ʈ
    int32 OtherBodyIndex,  // ��ġ�� �ٸ� ������Ʈ�� ��ü �ε���
    bool bFromSweep,  // �� ��ħ�� ���� ���� �߻��ߴ��� ����
    const FHitResult& SweepResult  // ���� ���� �߻��� ù �浹�� ���� ���
) {
    // �θ� Ŭ������ OnSphereOverlap �޼��带 ȣ���մϴ�.
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    // ��ġ�� �ٸ� ���͸� ABlasterCharacter�� ��ȯ(ĳ����) �õ��մϴ�.
    ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
    // �����ϸ� �Ʒ� �ڵ带 �����մϴ�.
    if (BlasterCharacter)
    {
        // BlasterCharacter�� Buff ������Ʈ�� ����ϴ�.
        UBuffComponent* Buff = BlasterCharacter->GetBuff();
        // Buff ������Ʈ�� ������ �Ʒ� �ڵ带 �����մϴ�.
        if (Buff)
        {
            // Buff�� ���� �ɷ��� ��ȭ�մϴ�.
            Buff->BuffJump(JumpZVelocityBuff, JumpBuffTime);
        }
    }

    // �� ��ü�� �ı��մϴ�.
    Destroy();
}
