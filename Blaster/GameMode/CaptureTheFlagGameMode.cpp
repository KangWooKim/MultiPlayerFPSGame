// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureTheFlagGameMode.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/CaptureTheFlag/FlagZone.h"
#include "Blaster/GameState/BlasterGameState.h"

// �÷��̾ ���ŵ� �� ȣ��Ǵ� �޼���
// ElimmedCharacter: ���ŵ� ĳ���Ϳ� ���� ����
// VictimController: ����� �÷��̾��� ��Ʈ�ѷ��� ���� ����
// AttackerController: ������ �÷��̾��� ��Ʈ�ѷ��� ���� ����
void ACaptureTheFlagGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// �⺻ ���� ����� PlayerEliminated �Լ��� ȣ���Ͽ� �⺻ ������ �����մϴ�.
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

// ����� ĸó�� �� ȣ��Ǵ� �޼���
// Flag: ĸó�� ��߿� ���� ����
// Zone: ����� ĸó�� ���� ���� ����
void ACaptureTheFlagGameMode::FlagCaptured(AFlag* Flag, AFlagZone* Zone)
{
	// ����� ���� ���� ���� �ٸ��ٸ� ��ȿ�� ĸó�� �����մϴ�.
	bool bValidCapture = Flag->GetTeam() != Zone->Team;

	// ���� ���� ���¸� �����ɴϴ�.
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(GameState);

	// ���� ���°� ��ȿ�� ���
	if (BGameState)
	{
		// ���� ���� �Ķ����� ���
		if (Zone->Team == ETeam::ET_BlueTeam)
		{
			// �Ķ����� ������ �ο��մϴ�.
			BGameState->BlueTeamScores();
		}
		// ���� ���� �������� ���
		if (Zone->Team == ETeam::ET_RedTeam)
		{
			// �������� ������ �ο��մϴ�.
			BGameState->RedTeamScores();
		}
	}
}

