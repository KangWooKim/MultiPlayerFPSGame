// Fill out your copyright notice in the Description page of Project Settings.


#include "ReturnToMainMenu.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Blaster/Character/BlasterCharacter.h"

// UReturnToMainMenu Ŭ������ �����մϴ�.

// MenuSetup �޼���
// �� �޼���� �޴� ������ ����մϴ�.
void UReturnToMainMenu::MenuSetup()
{
    // �޴��� ����Ʈ�� �߰��ϰ�, �޴��� ���̰� �����ϸ�, ��Ŀ���� ���� �� �ֵ��� �����մϴ�.
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    // ������ ���踦 �����ɴϴ�.
    UWorld* World = GetWorld();
    if (World)
    {
        // PlayerController�� null�̸� ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
        PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
        if (PlayerController)
        {
            // �Է� ��带 �����ϰ�, ���콺 Ŀ���� ���̰� �մϴ�.
            FInputModeGameAndUI InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }
    // ReturnButton�� ���ε��Ǿ� ���� ������ Ŭ�� �̺�Ʈ�� �߰��մϴ�.
    if (ReturnButton && !ReturnButton->OnClicked.IsBound())
    {
        ReturnButton->OnClicked.AddDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);
    }
    // ���� �ν��Ͻ��� �����ͼ� ��Ƽ�÷��̾� ���� ����ý����� �����ɴϴ�.
    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
        if (MultiplayerSessionsSubsystem)
        {
            // ���� �ı� �Ϸ� �̺�Ʈ�� �����ʸ� �߰��մϴ�.
            MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UReturnToMainMenu::OnDestroySession);
        }
    }
}

// Initialize �޼���
// �� �޼���� �ʱ�ȭ�� ����մϴ�.
bool UReturnToMainMenu::Initialize()
{
    // ���� Ŭ������ Initialize �޼��带 ȣ���Ͽ� �ʱ�ȭ�� �õ��մϴ�.
    if (!Super::Initialize())
    {
        // �ʱ�ȭ�� �����ϸ� false�� ��ȯ�մϴ�.
        return false;
    }

    // �ʱ�ȭ�� �����ϸ� true�� ��ȯ�մϴ�.
    return true;
}

// OnDestroySession �޼���
// �� �޼���� ���� �ı� �̺�Ʈ�� ó���մϴ�.
// �Ű�����:
// - bWasSuccessful: ���� �ı��� ���������� �̷�������� ��Ÿ���� bool ���Դϴ�.
void UReturnToMainMenu::OnDestroySession(bool bWasSuccessful)
{
    // ���� �ı��� �����ϸ� ReturnButton�� Ȱ��ȭ�ϰ� �޼��带 �����մϴ�.
    if (!bWasSuccessful)
    {
        ReturnButton->SetIsEnabled(true);
        return;
    }

    // ������ ���踦 �����ɴϴ�.
    UWorld* World = GetWorld();
    if (World)
    {
        // ���� ��带 �����ɴϴ�.
        AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
        if (GameMode)
        {
            // ���� ��尡 ������ ���� �޴��� ���ư��ϴ�.
            GameMode->ReturnToMainMenuHost();
        }
        else
        {
            // PlayerController�� null�̸� ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
            PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
            if (PlayerController)
            {
                // PlayerController�� ������ Ŭ���̾�Ʈ�� ���� �޴��� ���������ϴ�.
                PlayerController->ClientReturnToMainMenuWithTextReason(FText());
            }
        }
    }
}


// MenuTearDown �޼���
// �� �޼���� �޴��� �����ϴ� ������ �մϴ�.
void UReturnToMainMenu::MenuTearDown()
{
    // �θ� �������κ��� ���� ������ �����մϴ�.
    RemoveFromParent();

    // ������ ���踦 �����ɴϴ�.
    UWorld* World = GetWorld();
    if (World)
    {
        // PlayerController�� null�̸� ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
        PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
        if (PlayerController)
        {
            // ���Ӹ��� �Է� ��带 �����ϰ� ���콺 Ŀ���� ����ϴ�.
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }
    // ReturnButton�� ���ε��Ǿ� ������ Ŭ�� �̺�Ʈ�� �����մϴ�.
    if (ReturnButton && ReturnButton->OnClicked.IsBound())
    {
        ReturnButton->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);
    }
    // ��Ƽ�÷��̾� ���� ����ý����� �ְ� ���� �ı� �Ϸ� �̺�Ʈ�� ���ε��Ǿ� ������ �̺�Ʈ�� �����մϴ�.
    if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
    {
        MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UReturnToMainMenu::OnDestroySession);
    }
}

// ReturnButtonClicked �޼���
// �� �޼���� Return ��ư�� Ŭ���Ǿ��� ���� ������ �����մϴ�.
void UReturnToMainMenu::ReturnButtonClicked()
{
    // ReturnButton�� ��Ȱ��ȭ�մϴ�.
    ReturnButton->SetIsEnabled(false);

    // ������ ���踦 �����ɴϴ�.
    UWorld* World = GetWorld();
    if (World)
    {
        // ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
        APlayerController* FirstPlayerController = World->GetFirstPlayerController();
        if (FirstPlayerController)
        {
            // �÷��̾��� ĳ���͸� BlasterCharacter�� ĳ�����մϴ�.
            ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FirstPlayerController->GetPawn());
            if (BlasterCharacter)
            {
                // ĳ���Ͱ� �����ϸ� ������ �������� ��û�ϰ�, ������ ���� ���� ������ �����մϴ�.
                BlasterCharacter->ServerLeaveGame();
                BlasterCharacter->OnLeftGame.AddDynamic(this, &UReturnToMainMenu::OnPlayerLeftGame);
            }
            else
            {
                // ĳ���Ͱ� �������� ������ ReturnButton�� Ȱ��ȭ�մϴ�.
                ReturnButton->SetIsEnabled(true);
            }
        }
    }
}

// OnPlayerLeftGame �޼���
// �� �޼���� �÷��̾ ������ ������ ���� ������ �����մϴ�.
void UReturnToMainMenu::OnPlayerLeftGame()
{
    // �÷��̾ ������ ���� ���� �α׿� ����մϴ�.
    UE_LOG(LogTemp, Warning, TEXT("OnPlayerLeftGame()"))
        if (MultiplayerSessionsSubsystem)
        {
            // ��Ƽ�÷��̾� ���� ����ý����� �����ϸ� ������ �ı��մϴ�.
            UE_LOG(LogTemp, Warning, TEXT("MultiplayerSessionsSubsystem valid"))
                MultiplayerSessionsSubsystem->DestroySession();
        }
}

