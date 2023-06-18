// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

// �����ֱ� ���� �����Ǵ� �Ӽ��� �����ϴ� �޼���
void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ������ �Ӽ� ����
	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

// �ְ� ������ �����ϴ� �޼���
// ScoringPlayer : ������ �ø� �÷��̾� ����
void ABlasterGameState::UpdateTopScore(class ABlasterPlayerState* ScoringPlayer)
{
	// �ְ� ������ �޼��� �÷��̾ �����ϴ� ����
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

// �������� ������ ����� �� ȣ��Ǵ� �޼���
void ABlasterGameState::RedTeamScores()
{
	++RedTeamScore; // ������ ���� ����
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore); // HUD�� ������ ���� ����
	}
}

// ������� ������ ����� �� ȣ��Ǵ� �޼���
void ABlasterGameState::BlueTeamScores()
{
	++BlueTeamScore; // ����� ���� ����
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore); // HUD�� ����� ���� ����
	}
}

// ������ ������ �����Ǿ��� �� ȣ��Ǵ� �޼���
void ABlasterGameState::OnRep_RedTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore); // HUD�� ������ ���� ����
	}
}

// ����� ������ �����Ǿ��� �� ȣ��Ǵ� �޼���
void ABlasterGameState::OnRep_BlueTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore); // HUD�� ����� ���� ����
	}
}

