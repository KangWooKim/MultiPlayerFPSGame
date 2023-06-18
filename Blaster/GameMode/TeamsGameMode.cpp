// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"

// ATeamsGameMode�� ������
ATeamsGameMode::ATeamsGameMode()
{
	// �� ��ġ�� true�� �����մϴ�.
	bTeamsMatch = true;
}

// ���ο� �÷��̾ �α������� �� ȣ��Ǵ� �޼���
void ATeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	// �θ� Ŭ������ PostLogin �޼��� ȣ��
	Super::PostLogin(NewPlayer);

	// ���� ���¸� ��������, ABlasterGameState�� ĳ�����մϴ�.
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		// �÷��̾� ���¸� ��������, ABlasterPlayerState�� ĳ�����մϴ�.
		ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();

		// �÷��̾� ���°� ��ȿ�ϰ�, ���� �������� ���� ���
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			// ������� �ο� ���� ���������� ������ �������� �߰�
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
			// �׷��� ������ ������� �߰�
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

// �÷��̾ �α׾ƿ��� �� ȣ��Ǵ� �޼���
void ATeamsGameMode::Logout(AController* Exiting)
{
	// ���� ���¸� ��������, ABlasterGameState�� ĳ�����մϴ�.
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	// �÷��̾� ���¸� ��������, ABlasterPlayerState�� ĳ�����մϴ�.
	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		// �������� �ִ� ���, ���������� ����
		if (BGameState->RedTeam.Contains(BPState))
		{
			BGameState->RedTeam.Remove(BPState);
		}
		// ������� �ִ� ���, ��������� ����
		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}


// ��ġ�� ���۵Ǿ��� �� ȣ��Ǵ� �޼���
void ATeamsGameMode::HandleMatchHasStarted()
{
	// �θ� Ŭ������ HandleMatchHasStarted �޼��� ȣ��
	Super::HandleMatchHasStarted();

	// ���� ���¸� ��������, ABlasterGameState�� ĳ�����մϴ�.
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		// �÷��̾� �迭�� ��ȸ�մϴ�.
		for (auto PState : BGameState->PlayerArray)
		{
			// �÷��̾� ���¸� ��������, ABlasterPlayerState�� ĳ�����մϴ�.
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());

			// �÷��̾� ���°� ��ȿ�ϰ�, ���� �������� ���� ���
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				// ������� �ο� ���� ���������� ������ �������� �߰�
				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
				{
					BGameState->RedTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_RedTeam);
				}
				// �׷��� ������ ������� �߰�
				else
				{
					BGameState->BlueTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}
	}
}

// �����ڰ� �����ڿ��� ���� �������� ����ϴ� �޼���
// Attacker : �������� ��Ʈ�ѷ�
// Victim : �������� ��Ʈ�ѷ�
// BaseDamage : �⺻ ������ ��
float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	// �����ڿ� �������� �÷��̾� ���¸� �����ɴϴ�.
	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
	ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();

	// �����ڳ� �������� ���°� ��ȿ���� �ʴٸ�, �⺻ �������� ��ȯ�մϴ�.
	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;

	// �ڱ� �ڽ��� �����ϴ� ���, �⺻ �������� ��ȯ�մϴ�.
	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}

	// ���� ���� �÷��̾ �����ϴ� ���, �������� 0�Դϴ�.
	if (AttackerPState->GetTeam() == VictimPState->GetTeam())
	{
		return 0.f;
	}

	// �׷��� ���� ���, �⺻ �������� ��ȯ�մϴ�.
	return BaseDamage;
}


// �÷��̾ ���ŵ��� �� ȣ��Ǵ� �޼���
// ElimmedCharacter : ���ŵ� ĳ����
// VictimController : �������� �÷��̾� ��Ʈ�ѷ�
// AttackerController : �������� �÷��̾� ��Ʈ�ѷ�
void ATeamsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// �θ� Ŭ������ PlayerEliminated �޼��� ȣ��
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);

	// ���� ���¸� ��������, ABlasterGameState�� ĳ�����մϴ�.
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	// �������� �÷��̾� ���¸� �����ɴϴ�.
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;

	// ���� ���¿� �������� �÷��̾� ���°� ��ȿ�� ���
	if (BGameState && AttackerPlayerState)
	{
		// �����ڰ� ������̶��, ������� ������ �ø��ϴ�.
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		// �����ڰ� �������̶��, �������� ������ �ø��ϴ�.
		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}
