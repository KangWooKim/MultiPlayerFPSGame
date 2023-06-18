// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
// ĳ���� �������̸� ���� ����� ������ ������ Ŭ�����Դϴ�.
class BLASTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	// ĳ������ ü�¹��Դϴ�.
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// ĳ������ ü���� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	// ĳ������ ��ȣ�� ���Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;

	// ĳ������ ��ȣ���� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;

	// ĳ������ ������ ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreAmount;

	// �������� ������ ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RedTeamScore;

	// ������� ������ ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BlueTeamScore;

	// ������ ������ ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreSpacerText;

	// ĳ������ �й� Ƚ���� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefeatsAmount;

	// ĳ���Ͱ� ������ �ִ� ������ ź���� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoAmount;

	// ĳ���Ͱ� �����ϰ� �ִ� �߰� ź���� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoAmount;

	// ����� ���� �ð��� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountdownText;

	// ĳ���Ͱ� ������ �ִ� ����ź�� ���� ǥ���ϴ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrenadesText;

	// ���� ���� ��Ÿ���� �̹����Դϴ�.
	UPROPERTY(meta = (BindWidget))
	class UImage* HighPingImage;

	// ���� ���� �ִϸ��̼����� ��Ÿ���ϴ�.
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HighPingAnimation;
};

