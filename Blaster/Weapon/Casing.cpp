// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
// ź�� Ŭ���� ������
ACasing::ACasing()
{
	// �� Actor�� tick�� �ʿ� �����Ƿ� false�� �����մϴ�.
	PrimaryActorTick.bCanEverTick = false;

	// ź���� 3D �޽� ������Ʈ�� �����մϴ�.
	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	// �� �޽��� RootComponent�� �����մϴ�.
	SetRootComponent(CasingMesh);
	// ī�޶���� �浹 ������ �����ϵ��� �����մϴ�.
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);
	// ���� �ùķ��̼��� Ȱ��ȭ�մϴ�.
	CasingMesh->SetSimulatePhysics(true);
	// �߷��� Ȱ��ȭ�մϴ�.
	CasingMesh->SetEnableGravity(true);
	// ��ü �浹 �˸��� Ȱ��ȭ�մϴ�.
	CasingMesh->SetNotifyRigidBodyCollision(true);
	// ź�ǰ� �ѿ��� ���� ���� ���� �����մϴ�.
	ShellEjectionImpulse = 10.f;
}



void ACasing::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay�� ȣ���մϴ�.
	Super::BeginPlay();

	// ź�� �޽��� �浹 �� OnHit �Լ��� ȣ���ϵ��� �����մϴ�.
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	// �ѿ��� ź�ǰ� �������� �������� �ο��մϴ�.
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

// �浹 �� ȣ��Ǵ� �Լ�
void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ź�ǰ� �浹���� �� ���尡 �����Ǿ� �ִٸ� ���带 ����մϴ�.
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
	// ź�� Actor�� �ı��մϴ�.
	Destroy();
}

