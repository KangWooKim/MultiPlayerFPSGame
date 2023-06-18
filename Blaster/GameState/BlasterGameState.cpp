// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

// 생명주기 동안 복제되는 속성을 관리하는 메서드
void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 속성 지정
	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

// 최고 점수를 갱신하는 메서드
// ScoringPlayer : 점수를 올린 플레이어 상태
void ABlasterGameState::UpdateTopScore(class ABlasterPlayerState* ScoringPlayer)
{
	// 최고 점수를 달성한 플레이어를 관리하는 로직
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

// 레드팀이 점수를 얻었을 때 호출되는 메서드
void ABlasterGameState::RedTeamScores()
{
	++RedTeamScore; // 레드팀 점수 증가
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore); // HUD에 레드팀 점수 갱신
	}
}

// 블루팀이 점수를 얻었을 때 호출되는 메서드
void ABlasterGameState::BlueTeamScores()
{
	++BlueTeamScore; // 블루팀 점수 증가
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore); // HUD에 블루팀 점수 갱신
	}
}

// 레드팀 점수가 복제되었을 때 호출되는 메서드
void ABlasterGameState::OnRep_RedTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore); // HUD에 레드팀 점수 갱신
	}
}

// 블루팀 점수가 복제되었을 때 호출되는 메서드
void ABlasterGameState::OnRep_BlueTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore); // HUD에 블루팀 점수 갱신
	}
}

