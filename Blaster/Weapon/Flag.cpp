// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"

// �÷��� Ŭ������ ������
AFlag::AFlag()
{
    // �÷����� �޽� ������Ʈ�� �����ϰ� �ʱ�ȭ�մϴ�.
    FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));

    // �÷��� �޽��� ��Ʈ ������Ʈ�� �����մϴ�.
    SetRootComponent(FlagMesh);

    // ���� ���� �Ⱦ� ������ �÷��� �޽��� �����մϴ�.
    GetAreaSphere()->SetupAttachment(FlagMesh);
    GetPickupWidget()->SetupAttachment(FlagMesh);

    // ��� ä�ο� ���� �÷��� �޽��� �浹 ������ ���÷� �����մϴ�.
    FlagMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    // �÷��� �޽��� �浹 ����� ��Ȱ��ȭ�մϴ�.
    FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// �÷��װ� ��ӵ� ���� ������ �����ϴ� �޼���
void AFlag::Dropped()
{
    // �÷����� ���¸� ��ӵ� ���·� �����մϴ�.
    SetWeaponState(EWeaponState::EWS_Dropped);

    // �÷��� �޽��� ������ ������Ʈ���� �и��մϴ�.
    FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
    FlagMesh->DetachFromComponent(DetachRules);

    // �����ڸ� null�� �����մϴ�.
    SetOwner(nullptr);
    BlasterOwnerCharacter = nullptr;
    BlasterOwnerController = nullptr;
}

// �÷��׸� �����ϴ� �޼���
void AFlag::ResetFlag()
{
    // ���� �÷����� �����ڸ� �����ɴϴ�.
    ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner());

    if (FlagBearer)
    {
        // �÷��׸� �����ϰ� ���� �������� �����մϴ�.
        FlagBearer->SetHoldingTheFlag(false);
        FlagBearer->SetOverlappingWeapon(nullptr);

        // ĳ������ ��ũ�� ���¸� �����մϴ�.
        FlagBearer->UnCrouch();
    }

    // ���� ������ ���� ��� �Լ��� �����մϴ�.
    if (!HasAuthority()) return;

    FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
    FlagMesh->DetachFromComponent(DetachRules);
    SetWeaponState(EWeaponState::EWS_Initial);
    GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetAreaSphere()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Overlap);

    SetOwner(nullptr);
    BlasterOwnerCharacter = nullptr;
    BlasterOwnerController = nullptr;

    // �÷����� ��ġ�� ������ �ʱ� ���·� �����մϴ�.
    SetActorTransform(InitialTransform);
}

// �÷��װ� �����Ǿ��� ���� ������ �����ϴ� �޼���
void AFlag::OnEquipped()
{
    // �Ⱦ� ������ ����ϴ�.
    ShowPickupWidget(false);

    // ���� ���� �浹 ����� ��Ȱ��ȭ�մϴ�.
    GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // �÷��� �޽��� ���� �ùķ��̼ǰ� �߷��� ��Ȱ��ȭ�մϴ�.
    FlagMesh->SetSimulatePhysics(false);
    FlagMesh->SetEnableGravity(false);

    // �÷��� �޽��� �浹 ����� ���� �������� �����ϰ� ���� ���̳��� ä�ο� ���� �浹 ������ ���������� �����մϴ�.
    FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    FlagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECCollisionResponse::ECR_Overlap);

    // Ŀ���� ���̸� ��Ȱ��ȭ�մϴ�.
    EnableCustomDepth(false);
}


// �÷��װ� ��ӵǾ��� ���� ������ �����ϴ� �޼���
void AFlag::OnDropped()
{
    // ���� ������ �ִ� ���, ���� ���� �浹 ����� ���� �������� �����մϴ�.
    if (HasAuthority())
    {
        GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    // �÷��� �޽��� ���� �ùķ��̼ǰ� �߷��� Ȱ��ȭ�մϴ�.
    FlagMesh->SetSimulatePhysics(true);
    FlagMesh->SetEnableGravity(true);

    // �÷��� �޽��� �浹 ����� ���� �� ������ �����ϰ�, ��� ä�ο� ���� �浹 ������ �������� �����մϴ�.
    FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    FlagMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    // �÷��� �޽��� �÷��̾� ĳ���Ϳ� ī�޶� ä�ο� ���� �浹 ������ ���÷� �����մϴ�.
    FlagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECCollisionResponse::ECR_Ignore);
    FlagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);

    // �÷��� �޽��� ����� ���� ���� ���ٽ� ���� �Ķ������� �����ϰ� ���� ���¸� ������Ʈ�մϴ�.
    FlagMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
    FlagMesh->MarkRenderStateDirty();

    // ����� ���� ���̸� Ȱ��ȭ�մϴ�.
    EnableCustomDepth(true);
}

// ������ ���۵Ǿ��� ���� ������ �����ϴ� �޼���
void AFlag::BeginPlay()
{
    // ���� Ŭ������ BeginPlay �޼��带 ȣ���մϴ�.
    Super::BeginPlay();

    // �÷����� �ʱ� ��ȯ ���� �����ɴϴ�.
    InitialTransform = GetActorTransform();
}
