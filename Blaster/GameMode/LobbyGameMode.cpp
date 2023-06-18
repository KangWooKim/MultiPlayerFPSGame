// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"
// ���ο� �÷��̾ ���ӿ� �α������� �� ȣ��Ǵ� �Լ�
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	// �θ� Ŭ������ PostLogin �Լ� ȣ��
	Super::PostLogin(NewPlayer);

	// ���� ���� ���¿� ������ �÷��̾��� ���� �����ɴϴ�.
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	// ���� ���� �ν��Ͻ��� �����ɴϴ�.
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		// ��Ƽ�÷��̾� ���� ����ý����� �����ɴϴ�.
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		// ����ý����� ��ȿ���� üũ�մϴ�.
		check(Subsystem);

		// ���� �÷��̾� ���� ������ ���ϴ� ���� ���� ������ Ȯ���մϴ�.
		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
		{
			// ���� �ν��Ͻ��� �����ɴϴ�.
			UWorld* World = GetWorld();
			if (World)
			{
				// ���忡�� �ɸ��� Ʈ������ ����ϵ��� �����մϴ�.
				bUseSeamlessTravel = true;

				// ���ϴ� ��ġ Ÿ���� �����ɴϴ�.
				FString MatchType = Subsystem->DesiredMatchType;
				// ��ġ Ÿ�Կ� ���� ���� Ʈ������ �����մϴ�.
				if (MatchType == "FreeForAll")
				{
					// �����Ӱ� ��ο� �ο�� ������ �̵��մϴ�.
					World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
				}
				else if (MatchType == "Teams")
				{
					// ���� �����Ͽ� �ο�� ������ �̵��մϴ�.
					World->ServerTravel(FString("/Game/Maps/Teams?listen"));
				}
				else if (MatchType == "CaptureTheFlag")
				{
					// ��� ��� ����� ������ �̵��մϴ�.
					World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
				}
			}
		}
	}
}
