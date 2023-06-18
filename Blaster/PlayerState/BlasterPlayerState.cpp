// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"

// �÷��̾� ���¸� ������ �Ӽ� ����
void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    // ���� Ŭ������ �Ӽ��� ����
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // �� �� �Ӽ��� ��Ʈ��ũ�� ���� �����Ǿ�� ��
    DOREPLIFETIME(ABlasterPlayerState, Defeats);
    DOREPLIFETIME(ABlasterPlayerState, Team);
}

// �÷��̾� ���� ����
void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
    // �� ������ ScoreAmount�� �߰�
    SetScore(GetScore() + ScoreAmount);
    // ĳ���͸� ��� HUD�� ������ ����
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

// ������ ����� �� HUD�� ������Ʈ
void ABlasterPlayerState::OnRep_Score()
{
    // ���� Ŭ������ �Լ� ȣ��
    Super::OnRep_Score();

    // ĳ���͸� ��� HUD�� ������ ����
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

// �÷��̾� �й� Ƚ�� ����
void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
    // �� �й� Ƚ���� DefeatsAmount�� �߰�
    Defeats += DefeatsAmount;
    // ĳ���͸� ��� HUD�� �й� Ƚ���� ����
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

// �й� Ƚ���� ����� �� HUD�� ������Ʈ
void ABlasterPlayerState::OnRep_Defeats()
{
    // ĳ���͸� ��� HUD�� �й� Ƚ���� ����
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


// �÷��̾��� ���� �����ϴ� �޼���
void ABlasterPlayerState::SetTeam(ETeam TeamToSet)
{
    // ���޹��� ������ �÷��̾��� ���� ����
    Team = TeamToSet;

    // �÷��̾��� ĳ���͸� ���, ���� ���� ������ ����
    ABlasterCharacter* BCharacter = Cast <ABlasterCharacter>(GetPawn());
    if (BCharacter)
    {
        // ����: Team - ������ ���� ������ Ÿ��
        BCharacter->SetTeamColor(Team);
    }
}

// �� ������ ����� �� ȣ��Ǵ� �޼���
void ABlasterPlayerState::OnRep_Team()
{
    // �÷��̾��� ĳ���͸� ���, ���� ���� ������ ����
    ABlasterCharacter* BCharacter = Cast <ABlasterCharacter>(GetPawn());
    if (BCharacter)
    {
        // �� ������ �����ϴ� �κ�.
        BCharacter->SetTeamColor(Team);
    }
}

