// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "TeamsGameMode.generated.h"

/**
 * 
 */
UCLASS()
// �� ���� ��带 ��Ÿ���� Ŭ����
class BLASTER_API ATeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	// ������
	ATeamsGameMode();

	// ���ο� �÷��̾ �α������� �� ȣ��Ǵ� �޼���
	// NewPlayer : �α����� �÷��̾� ��Ʈ�ѷ�
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// �÷��̾ �α׾ƿ����� �� ȣ��Ǵ� �޼���
	// Exiting : �α׾ƿ��ϴ� ��Ʈ�ѷ�
	virtual void Logout(AController* Exiting) override;

	// �������� ����ϴ� �޼���
	// Attacker : ������ ��Ʈ�ѷ�
	// Victim : ������ ��Ʈ�ѷ�
	// BaseDamage : �⺻ ������ ��
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;

	// �÷��̾ ���ŵǾ��� �� ȣ��Ǵ� �޼���
	// ElimmedCharacter : ���ŵ� ĳ����
	// VictimController : �������� �÷��̾� ��Ʈ�ѷ�
	// AttackerController : �������� �÷��̾� ��Ʈ�ѷ�
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;

protected:
	// ��Ⱑ ���۵Ǿ��� �� ȣ��Ǵ� �޼���
	virtual void HandleMatchHasStarted() override;
};

