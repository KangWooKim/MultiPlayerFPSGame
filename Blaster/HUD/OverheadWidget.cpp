// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"

// UOverheadWidget Ŭ������ �����մϴ�.

// SetDisplayText �޼���
// �� �޼���� �ؽ�Ʈ�� �����ϴ� ������ �մϴ�.
// �Ű�����:
// - TextToDisplay: ȭ�鿡 ǥ���� ���ڿ��� FString �������� ���޹޽��ϴ�.
void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
    // DisplayText�� null�� �ƴ� ��쿡�� �����մϴ�.
    if (DisplayText)
    {
        // DisplayText�� �ؽ�Ʈ�� ���޹��� ���ڿ��� �����մϴ�.
        DisplayText->SetText(FText::FromString(TextToDisplay));
    }
}

// ShowPlayerNetRole �޼���
// �� �޼���� �÷��̾��� ��Ʈ��ũ ������ �����ִ� ������ �մϴ�.
// �Ű�����:
// - InPawn: ��Ʈ��ũ ������ ��ȸ�� APawn ��ü�� ���޹޽��ϴ�.
void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
    // InPawn�� RemoteRole ���� ��ȸ�մϴ�.
    ENetRole RemoteRole = InPawn->GetRemoteRole();
    FString Role;
    // RemoteRole�� ���� ���� Role ���ڿ��� �����մϴ�.
    switch (RemoteRole)
    {
    case ENetRole::ROLE_Authority:
        Role = FString("Authority");
        break;
    case ENetRole::ROLE_AutonomousProxy:
        Role = FString("Autonomous Proxy");
        break;
    case ENetRole::ROLE_SimulatedProxy:
        Role = FString("Simulated Proxy");
        break;
    case ENetRole::ROLE_None:
        Role = FString("None");
        break;
    }
    // Remote Role�� �׿� �����ϴ� Role ���ڿ��� �����մϴ�.
    FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
    // ������ ���ڿ��� ȭ�鿡 ǥ���մϴ�.
    SetDisplayText(RemoteRoleString);
}

// NativeDestruct �޼���
// �� �޼���� ������ �θ� �������κ��� �����ϰ� �⺻ �Ҹ��ڸ� ȣ���ϴ� ������ �մϴ�.
void UOverheadWidget::NativeDestruct()
{
    // ������ �θ� �������κ��� �����մϴ�.
    RemoveFromParent();
    // �⺻ �Ҹ��ڸ� ȣ���մϴ�.
    Super::NativeDestruct();
}

