// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenu.generated.h"

/**
 *
 */
UCLASS()
// UReturnToMainMenu Ŭ����
// �� Ŭ������ ���� �޴��� �����ϴ� �����Դϴ�.
class BLASTER_API UReturnToMainMenu : public UUserWidget
{
	// �� Ŭ������ ��Ÿ�����͸� �����մϴ�.
	GENERATED_BODY()

public:
	// �޴� ���� �޼����Դϴ�.
	void MenuSetup();

	// �޴� ���� �޼����Դϴ�.
	void MenuTearDown();

protected:
	// �ʱ�ȭ �޼����Դϴ�.
	// �� �޼���� UUserWidget Ŭ������ Initialize �޼��带 �������̵��մϴ�.
	virtual bool Initialize() override;

	// ���� �ı� �޼����Դϴ�.
	// �� �޼���� ������ ���������� �ı��Ǿ����� ���θ� �Ű������� �޽��ϴ�.
	UFUNCTION()
		void OnDestroySession(bool bWasSuccessful);

	// �÷��̾ ������ ���� ����� ������ �����ϴ� �޼����Դϴ�.
	UFUNCTION()
		void OnPlayerLeftGame();

private:
	// Return ��ư�Դϴ�.
	// �� ������ UButton Ŭ������ �ν��Ͻ��� �����մϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* ReturnButton;

	// Return ��ư Ŭ�� �޼����Դϴ�.
	UFUNCTION()
		void ReturnButtonClicked();

	// ��Ƽ�÷��̾� ���� ����ý����Դϴ�.
	// �� ������ UMultiplayerSessionsSubsystem Ŭ������ �ν��Ͻ��� �����մϴ�.
	UPROPERTY()
		class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	// �÷��̾� ��Ʈ�ѷ��Դϴ�.
	// �� ������ APlayerController Ŭ������ �ν��Ͻ��� �����մϴ�.
	UPROPERTY()
		class APlayerController* PlayerController;
};
