// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/GameState/BlasterGameState.h"

// �̸� ���ǵ� ��ġ ������ �̸��� �����մϴ�.
namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

// ABlasterGameMode�� ������
ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true; // ������ ���۵Ǳ� ���� �����̸� �����մϴ�.
}

// BeginPlay �޼���. ������ ���۵Ǹ� ȣ��˴ϴ�.
void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds(); // ���� ���� �ð��� ����մϴ�.
}

// Tick �޼���. �����Ӹ��� ȣ��˴ϴ�.
// @param DeltaTime: ���� �����Ӱ��� �ð� ����
void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart) // ��ġ�� �����ϱ⸦ ��ٸ��� ���̶��
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime; // ī��Ʈ�ٿ� �ð��� ������Ʈ�մϴ�.
		if (CountdownTime <= 0.f) // ī��Ʈ�ٿ��� ������
		{
			StartMatch(); // ��ġ�� �����մϴ�.
		}
	}
	else if (MatchState == MatchState::InProgress) // ��ġ�� ���� ���̶��
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime; // ī��Ʈ�ٿ� �ð��� ������Ʈ�մϴ�.
		if (CountdownTime <= 0.f) // ��ġ �ð��� ������
		{
			SetMatchState(MatchState::Cooldown); // ��ġ ���¸� Cooldown���� �����մϴ�.
		}
	}
	else if (MatchState == MatchState::Cooldown) // ��ٿ� ���̶��
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime; // ī��Ʈ�ٿ� �ð��� ������Ʈ�մϴ�.
		if (CountdownTime <= 0.f) // ��ٿ��� ������
		{
			RestartGame(); // ������ ������մϴ�.
		}
	}
}

// OnMatchStateSet �޼���. ��ġ ���°� �����Ǹ� ȣ��˴ϴ�.
void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	// ���� ���� ��� �÷��̾� ��Ʈ�ѷ��� �ݺ��ϸ�
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It); // ������ PlayerController�� ABlasterPlayerController�� ĳ�����մϴ�.
		if (BlasterPlayer) // ĳ������ �����ߴٸ�
		{
			BlasterPlayer->OnMatchStateSet(MatchState, bTeamsMatch); // �ش� �÷��̾��� OnMatchStateSet �޼��带 ȣ���մϴ�.
		}
	}
}

// CalculateDamage �޼���: �������� ����ϴ� �Լ��Դϴ�.
// @param Attacker: �������� ��Ʈ�ѷ�
// @param Victim: �������� ��Ʈ�ѷ�
// @param BaseDamage: �⺻ ������ ��
// ���� ���������� ������ ����� �����Ƿ� �⺻ �������� �״�� ��ȯ�մϴ�.
float ABlasterGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}

// PlayerEliminated �޼���: �÷��̾ �׾��� �� ȣ��Ǵ� �Լ��Դϴ�.
// @param ElimmedCharacter: ���� ĳ����
// @param VictimController: ���� �÷��̾��� ��Ʈ�ѷ�
// @param AttackerController: �������� ��Ʈ�ѷ�
void ABlasterGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// AttackerController Ȥ�� VictimController�� nullptr�̰ų� PlayerState�� nullptr�� ��� �Լ��� �����մϴ�.
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	// ���� ���¸� �����ɴϴ�.
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

	// �����ڰ� �����θ� ������ �ʾ�����, ���� ���°� �����ϴ� ���
	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{
		// ���� �����ϴ� �÷��̾� ����� �����ɴϴ�.
		TArray<ABlasterPlayerState*> PlayersCurrentlyInTheLead;
		for (auto LeadPlayer : BlasterGameState->TopScoringPlayers)
		{
			PlayersCurrentlyInTheLead.Add(LeadPlayer);
		}

		// �������� ������ ������Ű��, �ְ� ������ ������Ʈ�մϴ�.
		AttackerPlayerState->AddToScore(1.f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);

		// �����ڰ� �ְ� ������ ����� �÷��̾��̸�
		if (BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackerPlayerState->GetPawn());
			if (Leader)
			{
				Leader->MulticastGainedTheLead(); // �����μ� ���带 ������� �˸��ϴ�.
			}
		}

		// ������ ���������� ���� ������ �ƴ� �÷��̾�� ���带 �Ҿ��ٴ� �޽����� �����մϴ�.
		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
		{
			if (!BlasterGameState->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
			{
				ABlasterCharacter* Loser = Cast<ABlasterCharacter>(PlayersCurrentlyInTheLead[i]->GetPawn());
				if (Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}

	// �������� �й� Ƚ���� ������ŵ�ϴ�.
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	// ElimmedCharacter�� �����ϸ� �� ĳ���͸� �����մϴ�.
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	// ��� �÷��̾� ��Ʈ�ѷ����� ���� �̺�Ʈ�� �˸��ϴ�.
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		{
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		}
	}
}

// RequestRespawn �޼���: ĳ������ ������� ��û�մϴ�.
// @param ElimmedCharacter: ���� ĳ����
// @param ElimmedController: ���� ĳ������ ��Ʈ�ѷ�
void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	// ElimmedCharacter�� �����ϸ� �� ĳ���͸� �ʱ�ȭ�ϰ� �����մϴ�.
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}

	// ElimmedController�� �����ϸ� ���ο� ���� ��ġ���� �÷��̾ ������մϴ�.
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
// PlayerLeftGame �޼���: �÷��̾ ������ ������ �� ȣ��Ǵ� �Լ��Դϴ�.
// @param PlayerLeaving: ������ ������ �÷��̾��� ����
void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	// PlayerLeaving�� nullptr�� ��� �Լ��� �����մϴ�.
	if (PlayerLeaving == nullptr) return;

	// ������ ���� ���¸� �����ɴϴ�.
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

	// ������ �÷��̾ �ְ� ������ ����� �÷��̾� ��Ͽ� �־��ٸ� ��Ͽ��� �����մϴ�.
	if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}

	// ������ �÷��̾��� ĳ���͸� ������ Elim �Լ��� ȣ���Ͽ� �����մϴ�.
	ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}
