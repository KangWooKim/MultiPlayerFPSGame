// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
// UElimAnnouncement Ŭ������ ���� �˸��� ǥ���ϴ� ����� �����Դϴ�.
class BLASTER_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
    // �����ڿ� �������� �̸��� ���ڷ� �޾� ���� �˸� �ؽ�Ʈ�� �����մϴ�.
	void SetElimAnnouncementText(FString AttackerName, FString VictimName);

    // UHorizontalBox�� �����ϴ� ������ �˸� �޽����� �����ϴ� �ڽ��Դϴ�.
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* AnnouncementBox;

    // UTextBlock�� �����ϴ� ������ ���� �˸� �ؽ�Ʈ�� �����մϴ�.
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;

};
