// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponents/BuffComponent.h"

// AHealthPickup 클래스의 생성자입니다.
// 이 클래스의 객체가 네트워크에서 복제(replicate) 가능하도록 설정합니다.
AHealthPickup::AHealthPickup()
{
	bReplicates = true;
}

// 이 객체와 다른 객체가 겹치면 호출되는 메서드입니다.
void AHealthPickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent, // 이 객체와 겹친 컴포넌트
	AActor* OtherActor, // 이 객체와 겹친 다른 액터
	UPrimitiveComponent* OtherComp, // 이 객체와 겹친 다른 컴포넌트
	int32 OtherBodyIndex, // 겹친 다른 컴포넌트의 본체 인덱스
	bool bFromSweep, // 이 겹침이 스윕으로부터 발생한 것인지를 나타냄
	const FHitResult& SweepResult // 스윕 동안 발생한 첫 번째 충돌에 대한 결과
)
{
	// 부모 클래스의 OnSphereOverlap 메서드를 호출합니다.
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 겹친 다른 액터가 ABlasterCharacter인지 확인합니다.
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// 겹친 ABlasterCharacter의 BuffComponent를 가져옵니다.
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if (Buff)
		{
			// BuffComponent의 Heal 메서드를 호출하여 플레이어의 체력을 회복시킵니다.
			Buff->Heal(HealAmount, HealingTime);
		}
	}

	// 이 객체를 파괴합니다.
	Destroy();
}
