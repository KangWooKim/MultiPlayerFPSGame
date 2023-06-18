// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagZone.h"
#include "Components/SphereComponent.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/GameMode/CaptureTheFlagGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"

// AFlagZone ������
AFlagZone::AFlagZone()
{
	// �� ���Ͱ� ƽ�� ���� �� ������ ����
	PrimaryActorTick.bCanEverTick = false;

	// "ZoneSphere"��� �̸��� USphereComponent�� �����ϰ� ZoneSphere ������ �Ҵ�
	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	// �� ������ ��Ʈ ������Ʈ�� ZoneSphere�� ����
	SetRootComponent(ZoneSphere);
}

// BeginPlay �޼���: ������ ���۵� �� ȣ���
void AFlagZone::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay �޼��� ȣ��
	Super::BeginPlay();

	// ZoneSphere�� ������ �̺�Ʈ�� OnSphereOverlap �޼��带 ���ε�
	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlagZone::OnSphereOverlap);
}

// OnSphereOverlap �޼���: ZoneSphere�� �ٸ� ������Ʈ�� �������� �� ȣ���
// @param OverlappedComponent: ������ �̺�Ʈ�� �߻��� ������Ʈ
// @param OtherActor: �������� �ٸ� ����
// @param OtherComp: �������� �ٸ� ������Ʈ
// @param OtherBodyIndex: �ٸ� ������Ʈ�� �ٵ� �ε���
// @param bFromSweep: ���� ������� �������� �߻��ߴ����� ����
// @param SweepResult: ������ ����� �����ϰ� �ִ� ����ü
void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �������� ���Ͱ� AFlag Ÿ������ Ȯ��
	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	// �������� ���Ͱ� AFlag Ÿ���̸�, �� ���� ���� �÷��� ���� ���� �ƴ϶��
	if (OverlappingFlag && OverlappingFlag->GetTeam() != Team)
	{
		// ���� ������ ���� ��带 ACaptureTheFlagGameMode Ÿ������ ������
		ACaptureTheFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
		// ���� ��尡 �����ϸ� �÷��װ� �����Ǿ��ٴ� �̺�Ʈ�� ȣ��
		if (GameMode)
		{
			GameMode->FlagCaptured(OverlappingFlag, this);
		}
		// �������� �÷��׸� ����
		OverlappingFlag->ResetFlag();
	}
}

