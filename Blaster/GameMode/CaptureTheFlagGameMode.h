// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamsGameMode.h"
#include "CaptureTheFlagGameMode.generated.h"

/**
 * 
 */
UCLASS()
// Capture The Flag ���� ��� Ŭ����
class BLASTER_API ACaptureTheFlagGameMode : public ATeamsGameMode
{
	GENERATED_BODY()

public:
	// �÷��̾ ���ӿ��� ���ܵ� �� ȣ��Ǵ� �޼���
	// ElimmedCharacter: ���ӿ��� ���ܵ� ĳ���Ϳ� ���� ����
	// VictimController: ����� �÷��̾��� ��Ʈ�ѷ��� ���� ����
	// AttackerController: ������ �÷��̾��� ��Ʈ�ѷ��� ���� ����
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;

	// ����� ĸó�� �� ȣ��Ǵ� �޼���
	// Flag: ĸó�� ��߿� ���� ����
	// Zone: ����� ĸó�� ���� ���� ����
	void FlagCaptured(class AFlag* Flag, class AFlagZone* Zone);
};

