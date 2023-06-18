// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"

// 플레이어 상태를 재현할 속성 설정
void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    // 상위 클래스의 속성도 포함
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 이 두 속성이 네트워크를 통해 복제되어야 함
    DOREPLIFETIME(ABlasterPlayerState, Defeats);
    DOREPLIFETIME(ABlasterPlayerState, Team);
}

// 플레이어 점수 증가
void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
    // 총 점수에 ScoreAmount를 추가
    SetScore(GetScore() + ScoreAmount);
    // 캐릭터를 얻고 HUD에 점수를 설정
    Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
    if (Character)
    {
        Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
        if (Controller)
        {
            Controller->SetHUDScore(GetScore());
        }
    }
}

// 점수가 변경될 때 HUD를 업데이트
void ABlasterPlayerState::OnRep_Score()
{
    // 상위 클래스의 함수 호출
    Super::OnRep_Score();

    // 캐릭터를 얻고 HUD에 점수를 설정
    Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
    if (Character)
    {
        Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
        if (Controller)
        {
            Controller->SetHUDScore(GetScore());
        }
    }
}

// 플레이어 패배 횟수 증가
void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
    // 총 패배 횟수에 DefeatsAmount를 추가
    Defeats += DefeatsAmount;
    // 캐릭터를 얻고 HUD에 패배 횟수를 설정
    Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
    if (Character)
    {
        Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
        if (Controller)
        {
            Controller->SetHUDDefeats(Defeats);
        }
    }
}

// 패배 횟수가 변경될 때 HUD를 업데이트
void ABlasterPlayerState::OnRep_Defeats()
{
    // 캐릭터를 얻고 HUD에 패배 횟수를 설정
    Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
    if (Character)
    {
        Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
        if (Controller)
        {
            Controller->SetHUDDefeats(Defeats);
        }
    }
}


// 플레이어의 팀을 설정하는 메서드
void ABlasterPlayerState::SetTeam(ETeam TeamToSet)
{
    // 전달받은 팀으로 플레이어의 팀을 설정
    Team = TeamToSet;

    // 플레이어의 캐릭터를 얻고, 팀에 따른 색상을 설정
    ABlasterCharacter* BCharacter = Cast <ABlasterCharacter>(GetPawn());
    if (BCharacter)
    {
        // 인자: Team - 설정할 팀의 열거형 타입
        BCharacter->SetTeamColor(Team);
    }
}

// 팀 설정이 변경될 때 호출되는 메서드
void ABlasterPlayerState::OnRep_Team()
{
    // 플레이어의 캐릭터를 얻고, 팀에 따른 색상을 설정
    ABlasterCharacter* BCharacter = Cast <ABlasterCharacter>(GetPawn());
    if (BCharacter)
    {
        // 팀 색상을 설정하는 부분.
        BCharacter->SetTeamColor(Team);
    }
}

