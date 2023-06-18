// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
// UOverheadWidget Ŭ������ �����մϴ�. �� Ŭ������ UUserWidget�� ��ӹ޽��ϴ�.
class BLASTER_API UOverheadWidget : public UUserWidget
{
    // Ŭ���� �ٵ� �����մϴ�.
    GENERATED_BODY()

public:
    // DisplayText��� UTextBlock ������ ������ �����մϴ�. 
    // �� ������ ������ ǥ�õ� �ؽ�Ʈ�� �����մϴ�.
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DisplayText;

    // SetDisplayText �޼��带 �����մϴ�.
    // �� �޼���� ������ ǥ�õ� �ؽ�Ʈ�� �����մϴ�.
    // �Ű�����:
    // - TextToDisplay: ȭ�鿡 ǥ���� ���ڿ��� FString �������� ���޹޽��ϴ�.
    void SetDisplayText(FString TextToDisplay);

    // ShowPlayerNetRole �޼��带 �����մϴ�.
    // �� �޼���� �������Ʈ���� ȣ�� �����ϸ�, �÷��̾��� ��Ʈ��ũ ������ �����ݴϴ�.
    // �Ű�����:
    // - InPawn: ��Ʈ��ũ ������ ��ȸ�� APawn ��ü�� ���޹޽��ϴ�.
    UFUNCTION(BlueprintCallable)
    void ShowPlayerNetRole(APawn* InPawn);

protected:
    // NativeDestruct �޼��带 �����մϴ�.
    // �� �޼���� ������ �θ� �������κ��� �����ϰ� �⺻ �Ҹ��ڸ� ȣ���մϴ�.
    virtual void NativeDestruct() override;

};
