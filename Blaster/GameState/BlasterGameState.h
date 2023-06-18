// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
// Blaster ���� ���� Ŭ����
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()
public:
	// �����Ǵ� �Ӽ����� �����ϴ� �޼���
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// �ְ� ������ �����ϴ� �޼���
	// ScoringPlayer : ������ �ø� �÷��̾� ����
	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer);

	// �����Ǵ� �Ӽ�, �ְ� ������ ���� �÷��̾� ����Ʈ
	UPROPERTY(Replicated)
		TArray<ABlasterPlayerState*> TopScoringPlayers;

	/**
	* �� ���� �޼���� ����
	*/

	// �������� ������ ����� �� ȣ��Ǵ� �޼���
	void RedTeamScores();

	// ������� ������ ����� �� ȣ��Ǵ� �޼���
	void BlueTeamScores();

	// ������ �÷��̾� ����Ʈ
	TArray<ABlasterPlayerState*> RedTeam;

	// ����� �÷��̾� ����Ʈ
	TArray<ABlasterPlayerState*> BlueTeam;

	// �����Ǵ� �Ӽ�, �������� ���� ����
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
		float RedTeamScore = 0.f;

	// �����Ǵ� �Ӽ�, ������� ���� ����
	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
		float BlueTeamScore = 0.f;

	// ������ ������ �����Ǿ��� �� ȣ��Ǵ� �޼���
	UFUNCTION()
		void OnRep_RedTeamScore();

	// ����� ������ �����Ǿ��� �� ȣ��Ǵ� �޼���
	UFUNCTION()
		void OnRep_BlueTeamScore();

private:
	// ���� �ְ� ����
	float TopScore = 0.f;
};

