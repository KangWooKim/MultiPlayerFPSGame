// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
 // �÷��̾��� ���¸� ��Ÿ���� Ŭ����
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	// ������Ÿ�� ���� �����ؾ��ϴ� �Ӽ��� �����ɴϴ�. 
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	/**
	* ���� �˸�
	*/
	// ������ ������ �� ȣ��Ǵ� �Լ�
	virtual void OnRep_Score() override;

	// �й谡 ������ �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		virtual void OnRep_Defeats();

	// ������ �߰��ϴ� �Լ�, ScoreAmount - �߰��� ������ ��
	void AddToScore(float ScoreAmount);

	// �й踦 �߰��ϴ� �Լ�, DefeatsAmount - �߰��� �й��� ��
	void AddToDefeats(int32 DefeatsAmount);

private:
	// �÷��̾� ĳ������ ����
	UPROPERTY()
		class ABlasterCharacter* Character;

	// �÷��̾� ��Ʈ�ѷ��� ����
	UPROPERTY()
		class ABlasterPlayerController* Controller;

	// �й��� ��, ���� �� OnRep_Defeats�� ȣ���
	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
		int32 Defeats;

	// �÷��̾��� ��, ���� �� OnRep_Team�� ȣ���
	UPROPERTY(ReplicatedUsing = OnRep_Team)
		ETeam Team = ETeam::ET_NoTeam;

	// ���� ������ �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnRep_Team();

public:
	// ���� �������� �ζ��� �Լ�, ��ȯ��: ���� ��
	FORCEINLINE ETeam GetTeam() const { return Team; }

	// ���� �����ϴ� �Լ�, TeamToSet - ������ ��
	void SetTeam(ETeam TeamToSet);
};
