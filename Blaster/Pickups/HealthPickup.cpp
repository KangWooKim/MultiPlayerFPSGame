// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

// AHealthPickup Ŭ������ �������Դϴ�.
// �� Ŭ������ ��ü�� ��Ʈ��ũ���� ����(replicate) �����ϵ��� �����մϴ�.
AHealthPickup::AHealthPickup()
{
	bReplicates = true;
}

// �� ��ü�� �ٸ� ��ü�� ��ġ�� ȣ��Ǵ� �޼����Դϴ�.
void AHealthPickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent, // �� ��ü�� ��ģ ������Ʈ
	AActor* OtherActor, // �� ��ü�� ��ģ �ٸ� ����
	UPrimitiveComponent* OtherComp, // �� ��ü�� ��ģ �ٸ� ������Ʈ
	int32 OtherBodyIndex, // ��ģ �ٸ� ������Ʈ�� ��ü �ε���
	bool bFromSweep, // �� ��ħ�� �������κ��� �߻��� �������� ��Ÿ��
	const FHitResult& SweepResult // ���� ���� �߻��� ù ��° �浹�� ���� ���
)
{
	// �θ� Ŭ������ OnSphereOverlap �޼��带 ȣ���մϴ�.
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// ��ģ �ٸ� ���Ͱ� ABlasterCharacter���� Ȯ���մϴ�.
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// ��ģ ABlasterCharacter�� BuffComponent�� �����ɴϴ�.
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if (Buff)
		{
			// BuffComponent�� Heal �޼��带 ȣ���Ͽ� �÷��̾��� ü���� ȸ����ŵ�ϴ�.
			Buff->Heal(HealAmount, HealingTime);
		}
	}

	// �� ��ü�� �ı��մϴ�.
	Destroy();
}
