// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/HUD/Announcement.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Components/Image.h"
#include "Blaster/HUD/ReturnToMainMenu.h"
#include "Blaster/BlasterTypes/Announcement.h"

// �÷��̾� ��Ʈ�ѷ��� ���� Ŭ������ �����մϴ�.
void ABlasterPlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	// Ŭ���̾�Ʈ�κ��� �����ڿ� �����ڿ� ���� ������ �޾� ó���մϴ�.
	ClientElimAnnouncement(Attacker, Victim);
}

// Ŭ���̾�Ʈ�κ��� Elim(Ż��) �˸��� �����մϴ�.
void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	// ���� �÷��̾��� ���¸� �����ɴϴ�.
	APlayerState* Self = GetPlayerState<APlayerState>();

	// ������, ������, �׸��� ���� �÷��̾ ��� ������ ���
	if (Attacker && Victim && Self)
	{
		// BlasterHUD�� �������ų� ���� HUD�� BlasterHUD�� ĳ�����մϴ�.
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

		// BlasterHUD�� ������ ���
		if (BlasterHUD)
		{
			// ���� �÷��̾ �������̸�, �����ڴ� �ƴ� ���
			if (Attacker == Self && Victim != Self)
			{
				// �����ڿ� �������� �̸��� Elim �˸����� �߰��մϴ�.
				BlasterHUD->AddElimAnnouncement("You", Victim->GetPlayerName());
				return;
			}
			// ���� �÷��̾ �������̸�, �����ڴ� �ƴ� ���
			if (Victim == Self && Attacker != Self)
			{
				// �����ڿ� �������� �̸��� Elim �˸����� �߰��մϴ�.
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "you");
				return;
			}
			// �����ڿ� �����ڰ� �����ϰ�, �װ��� ���� �÷��̾��� ���
			if (Attacker == Victim && Attacker == Self)
			{
				// �����ڿ� �������� �̸��� Elim �˸����� �߰��մϴ�.
				BlasterHUD->AddElimAnnouncement("You", "yourself");
				return;
			}
			// �����ڿ� �����ڰ� �����ϳ�, �װ��� ���� �÷��̾�� �ƴ� ���
			if (Attacker == Victim && Attacker != Self)
			{
				// �����ڿ� �������� �̸��� Elim �˸����� �߰��մϴ�.
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "themselves");
				return;
			}

			// �� ���� ���, �����ڿ� �������� �̸��� Elim �˸����� �߰��մϴ�.
			BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), Victim->GetPlayerName());
		}
	}
}

// �÷��� ���� �� ����Ǵ� �Լ��Դϴ�.
void ABlasterPlayerController::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay �Լ��� �����մϴ�.
	Super::BeginPlay();

	// ���� HUD�� BlasterHUD�� ĳ�����մϴ�.
	BlasterHUD = Cast<ABlasterHUD>(GetHUD());

	// ������ ��ġ ���¸� Ȯ���մϴ�.
	ServerCheckMatchState();
}

// ���ø����̼ǿ� �ʿ��� �Ӽ����� �����մϴ�.
void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// ���� Ŭ������ ���� �Լ��� ȣ���մϴ�.
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// MatchState�� bShowTeamScores ������ ���ø����̼ǿ� ���Խ�ŵ�ϴ�.
	DOREPLIFETIME(ABlasterPlayerController, MatchState);
	DOREPLIFETIME(ABlasterPlayerController, bShowTeamScores);
}

// �� ������ ����� �޼����Դϴ�.
void ABlasterPlayerController::HideTeamScores()
{
	// BlasterHUD�� �������ų� ���� HUD�� BlasterHUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD�� ��ȿ���� �˻��մϴ�.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore &&
		BlasterHUD->CharacterOverlay->ScoreSpacerText;

	// HUD�� ��ȿ�� ���, �� ������ �ʱ�ȭ�մϴ�.
	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->ScoreSpacerText->SetText(FText());
	}
}

// �� ������ �ʱ�ȭ�ϴ� �޼����Դϴ�.
void ABlasterPlayerController::InitTeamScores()
{
	// BlasterHUD�� �������ų� ���� HUD�� BlasterHUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD�� ��ȿ���� �˻��մϴ�.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore &&
		BlasterHUD->CharacterOverlay->ScoreSpacerText;

	// HUD�� ��ȿ�� ���, �� ������ �ʱ�ȭ�մϴ�.
	if (bHUDValid)
	{
		FString Zero("0");
		FString Spacer("|");
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->ScoreSpacerText->SetText(FText::FromString(Spacer));
	}
}


// HUD�� ������ ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
	// BlasterHUD�� �������ų� ���� HUD�� BlasterHUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD�� ��ȿ���� �˻��մϴ�.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore;

	// HUD�� ��ȿ�� ���, ������ ������ �����մϴ�.
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), RedScore);
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(ScoreText));
	}
}

// HUD�� �Ķ��� ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
	// BlasterHUD�� �������ų� ���� HUD�� BlasterHUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD�� ��ȿ���� �˻��մϴ�.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->BlueTeamScore;

	// HUD�� ��ȿ�� ���, �Ķ��� ������ �����մϴ�.
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(ScoreText));
	}
}

// �� ƽ���� ����Ǵ� �޼����Դϴ�.
void ABlasterPlayerController::Tick(float DeltaTime)
{
	// ���� Ŭ������ Tick �޼��带 ȣ���մϴ�.
	Super::Tick(DeltaTime);

	// HUD�� �ð��� �����ϰ�, �ð� ����ȭ�� Ȯ���մϴ�.
	SetHUDTime();
	CheckTimeSync(DeltaTime);

	// �ʱ�ȭ ���¸� Ȯ���մϴ�.
	PollInit();

	// ��Ʈ��ũ �����ð��� Ȯ���մϴ�.
	CheckPing(DeltaTime);
}


// ��Ʈ��ũ �����ð� (��)�� Ȯ���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::CheckPing(float DeltaTime)
{
	// ���� ������ �ִ� ��� �� �޼��带 �����մϴ�.
	if (HasAuthority()) return;

	// �� üũ�� ���� ���� �ð��� ������ŵ�ϴ�.
	HighPingRunningTime += DeltaTime;

	// �� üũ �ֱⰡ �Ǿ����� Ȯ���մϴ�.
	if (HighPingRunningTime > CheckPingFrequency)
	{
		// PlayerState�� �������ų� ���� PlayerState�� ĳ�����մϴ�.
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState;

		// PlayerState�� ��ȿ�� ���, ���� Ȯ���մϴ�.
		if (PlayerState)
		{
			// ���� �ʹ� ���� ��� ��� ����ϰ�, ���¸� ������ �����մϴ�.
			if (PlayerState->GetPing() * 4 > HighPingThreshold) // ���� ����Ǿ� �ֽ��ϴ�. ���� ���� �� / 4 �Դϴ�.
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				// ���� ���� ���� ���, ���¸� ������ �����մϴ�.
				ServerReportPingStatus(false);
			}
		}

		// �� üũ�� ���� ���� �ð��� �ʱ�ȭ�մϴ�.
		HighPingRunningTime = 0.f;
	}

	// �� ��� �ִϸ��̼� ��� ���¸� Ȯ���մϴ�.
	bool bHighPingAnimationPlaying =
		BlasterHUD && BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingAnimation &&
		BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation);

	// �� ��� �ִϸ��̼� ��� ���� ���, ���� �ð��� �˻��Ͽ� �����մϴ�.
	if (bHighPingAnimationPlaying)
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

// ���� �޴��� ���ư��� �ɼ��� �����ִ� �޼����Դϴ�.
void ABlasterPlayerController::ShowReturnToMainMenu()
{
	// ���� �޴� ������ ���� ��� �� �޼��带 �����մϴ�.
	if (ReturnToMainMenuWidget == nullptr) return;

	// ���� �޴��� �����ϰų� �����ϴ� �޴��� �����ɴϴ�.
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}

	// ���� �޴��� ��ȿ�� ���, �޴��� ǥ���ϰų� �����մϴ�.
	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
}


// �� ���� ǥ�� ���¸� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::OnRep_ShowTeamScores()
{
	// bShowTeamScores ���� ���� �� ������ �ʱ�ȭ�ϰų� ����ϴ�.
	if (bShowTeamScores)
	{
		InitTeamScores();  // �� ������ �ʱ�ȭ�մϴ�.
	}
	else
	{
		HideTeamScores();  // �� ������ ����ϴ�.
	}
}

// ���� �ʹ� ������ ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::ServerReportPingStatus_Implementation(bool bHighPing)  // bHighPing: ���� ������ �������� ��Ÿ���� �Ҹ����Դϴ�.
{
	HighPingDelegate.Broadcast(bHighPing);  // ���� �� ���¸� ��ε�ĳ��Ʈ�մϴ�.
}

// �������� �ð� ����ȭ�� Ȯ���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::CheckTimeSync(float DeltaTime)  // DeltaTime: ���� �����ӿ��� ���� �����ӱ��� �ɸ� �ð��Դϴ�.
{
	TimeSyncRunningTime += DeltaTime;

	// ���� ��Ʈ�ѷ��̰�, ����ȭ �ֱⰡ ���������, ������ ���� �ð��� ��û�մϴ�.
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;  // ����ȭ�� ���� ���� �ð��� �ʱ�ȭ�մϴ�.
	}
}

// ���� �� ��� ǥ���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::HighPingWarning()
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// BlasterHUD�� ��ȿ�ϸ�, ���� �� �ִϸ��̼��� ǥ���մϴ�.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(1.f);  // �� �̹����� �������� �����մϴ�.

		// �� �ִϸ��̼��� ����մϴ�.
		BlasterHUD->CharacterOverlay->PlayAnimation(
			BlasterHUD->CharacterOverlay->HighPingAnimation,
			0.f,
			5);
	}
}


// ���� �� ��� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::StopHighPingWarning()
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);  // �� �̹����� �������� 0���� �����մϴ�.

		if (BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation))
		{
			// ���� �� �ִϸ��̼��� ��� ���̸� �����մϴ�.
			BlasterHUD->CharacterOverlay->StopAnimation(BlasterHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}

// ���� ����� ���¸� üũ�ϴ� �޼����Դϴ�.
void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));  // ���� ���� ��带 �����ɴϴ�.

	if (GameMode)
	{
		// ���� ���κ��� ������ ���� ���� ������ �����մϴ�.
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();

		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

// Ŭ���̾�Ʈ�� �߰��� ���ӿ� �����ϴ� ��츦 ó���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	// �� �Ű����� ���� �����մϴ�.
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;

	OnMatchStateSet(MatchState);  // ���� ���¸� �����մϴ�.

	if (BlasterHUD && MatchState == MatchState::WaitingToStart)
	{
		BlasterHUD->AddAnnouncement();  // ������ ���� ��� ���̸� �˸��� �߰��մϴ�.
	}
}

// ���ο� Pawn�� ������ �� ȣ��Ǵ� �޼����Դϴ�.
void ABlasterPlayerController::OnPossess(APawn* InPawn)  // InPawn: ������ ���ο� Pawn�Դϴ�.
{
	Super::OnPossess(InPawn);  // �θ� Ŭ������ OnPossess �޼��带 ȣ���մϴ�.

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn);  // InPawn�� ABlasterCharacter�� ĳ�����մϴ�.

	if (BlasterCharacter)
	{
		SetHUDHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());  // HUD�� ü���� �����մϴ�.
	}
}


// HUD�� ü���� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth) // Health: ������ ü�� ��, MaxHealth: ü���� �ִ� ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HealthBar &&
		BlasterHUD->CharacterOverlay->HealthText;

	if (bHUDValid)
	{
		// ü���� �ۼ�Ʈ�� ����ϰ�, HealthBar�� HealthText�� ������Ʈ�մϴ�.
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		// ���� HUD�� �ʱ�ȭ���� �ʾҴٸ�, �ʱ�ȭ�� �ؾ��ϴ� ������ ǥ���ϰ� ü�� ���� �����մϴ�.
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

// HUD�� ���� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDShield(float Shield, float MaxShield) // Shield: ������ �� ��, MaxShield: ���� �ִ� ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ShieldBar &&
		BlasterHUD->CharacterOverlay->ShieldText;

	if (bHUDValid)
	{
		// ���� �ۼ�Ʈ�� ����ϰ�, ShieldBar�� ShieldText�� ������Ʈ�մϴ�.
		const float ShieldPercent = Shield / MaxShield;
		BlasterHUD->CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		FString ShieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		BlasterHUD->CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
	}
	else
	{
		// ���� HUD�� �ʱ�ȭ���� �ʾҴٸ�, �ʱ�ȭ�� �ؾ��ϴ� ������ ǥ���ϰ� �� ���� �����մϴ�.
		bInitializeShield = true;
		HUDShield = Shield;
		HUDMaxShield = MaxShield;
	}
}

// HUD�� ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDScore(float Score) // Score: ������ ���� ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ScoreAmount;

	if (bHUDValid)
	{
		// ScoreAmount�� ������Ʈ�մϴ�.
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		// ���� HUD�� �ʱ�ȭ���� �ʾҴٸ�, �ʱ�ȭ�� �ؾ��ϴ� ������ ǥ���ϰ� ���� ���� �����մϴ�.
		bInitializeScore = true;
		HUDScore = Score;
	}
}


// HUD�� Defeats ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDDefeats(int32 Defeats) // Defeats: ������ Defeats ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->DefeatsAmount;

	if (bHUDValid)
	{
		// DefeatsAmount�� ������Ʈ�մϴ�.
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		BlasterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		// ���� HUD�� �ʱ�ȭ���� �ʾҴٸ�, �ʱ�ȭ�� �ؾ��ϴ� ������ ǥ���ϰ� Defeats ���� �����մϴ�.
		bInitializeDefeats = true;
		HUDDefeats = Defeats;
	}
}

// HUD�� ������ ź���� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDWeaponAmmo(int32 Ammo) // Ammo: ������ ������ ź�� ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount;

	if (bHUDValid)
	{
		// WeaponAmmoAmount�� ������Ʈ�մϴ�.
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		// ���� HUD�� �ʱ�ȭ���� �ʾҴٸ�, �ʱ�ȭ�� �ؾ��ϴ� ������ ǥ���ϰ� ������ ź�� ���� �����մϴ�.
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

// HUD�� ������ ź���� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo) // Ammo: ������ ������ ź�� ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount;

	if (bHUDValid)
	{
		// CarriedAmmoAmount�� ������Ʈ�մϴ�.
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		// ���� HUD�� �ʱ�ȭ���� �ʾҴٸ�, �ʱ�ȭ�� �ؾ��ϴ� ������ ǥ���ϰ� ������ ź�� ���� �����մϴ�.
		bInitializeCarriedAmmo = true;
		HUDCarriedAmmo = Ammo;
	}
}

// HUD�� ��� ī��Ʈ�ٿ� ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime) // CountdownTime: ������ ī��Ʈ�ٿ� �ð�(��)
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->MatchCountdownText;

	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			// ī��Ʈ�ٿ� �ð��� 0 �̸��� ��� �ؽ�Ʈ�� ���ϴ�.
			BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}

		// �а� �ʷ� ��ȯ�մϴ�.
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		// ī��Ʈ�ٿ� �ؽ�Ʈ�� ������Ʈ�մϴ�.
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

// HUD�� ���� ī��Ʈ�ٿ� ������ �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime) // CountdownTime: ������ ī��Ʈ�ٿ� �ð�(��)
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->Announcement &&
		BlasterHUD->Announcement->WarmupTime;

	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			// ī��Ʈ�ٿ� �ð��� 0 �̸��� ��� �ؽ�Ʈ�� ���ϴ�.
			BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		// �а� �ʷ� ��ȯ�մϴ�.
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		// ī��Ʈ�ٿ� �ؽ�Ʈ�� ������Ʈ�մϴ�.
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

// HUD�� ����ź ���� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDGrenades(int32 Grenades) // Grenades: ������ ����ź�� ��
{
	// BlasterHUD�� �������ų� ���� HUD�� ĳ�����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->GrenadesText;

	if (bHUDValid)
	{
		// ����ź�� ���� �ؽ�Ʈ�� ��ȯ�մϴ�.
		FString GrenadesText = FString::Printf(TEXT("%d"), Grenades);
		BlasterHUD->CharacterOverlay->GrenadesText->SetText(FText::FromString(GrenadesText));
	}
	else
	{
		// ����ź�� ���� �ʱ�ȭ�մϴ�.
		bInitializeGrenades = true;
		HUDGrenades = Grenades;
	}
}


// HUD�� ���� �ð��� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;

	// ���� ���� ���¿� ���� ���� �ð��� ����մϴ�.
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft); // ���� �ʷ� ��ȯ�մϴ�.

	if (HasAuthority()) // �������� ����Ǵ� ���
	{
		if (BlasterGameMode == nullptr)
		{
			// ���� ��带 �������ų� ���� ���� ��带 ĳ�����մϴ�.
			BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
			LevelStartingTime = BlasterGameMode->LevelStartingTime;
		}
		BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
		if (BlasterGameMode)
		{
			SecondsLeft = FMath::CeilToInt(BlasterGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			// ���� ī��Ʈ�ٿ��� �����մϴ�.
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			// ��� ī��Ʈ�ٿ��� �����մϴ�.
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

// �ʱ�ȭ ��û�� Ȯ���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (BlasterHUD && BlasterHUD->CharacterOverlay)
		{
			CharacterOverlay = BlasterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				// �� ��Ұ� �ʱ�ȭ�Ǿ�� �ϴ� ��� �ش� ����� HUD�� �����մϴ�.
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeShield) SetHUDShield(HUDShield, HUDMaxShield);
				if (bInitializeScore) SetHUDScore(HUDScore);
				if (bInitializeDefeats) SetHUDDefeats(HUDDefeats);
				if (bInitializeCarriedAmmo) SetHUDCarriedAmmo(HUDCarriedAmmo);
				if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);

				ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
				if (BlasterCharacter && BlasterCharacter->GetCombat())
				{
					if (bInitializeGrenades) SetHUDGrenades(BlasterCharacter->GetCombat()->GetGrenades());
				}
			}
		}
	}
}


// �Է� ������Ҹ� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); // �θ� Ŭ������ SetupInputComponent �޼��带 ȣ���մϴ�.
	if (InputComponent == nullptr) return; // �Է� ������Ұ� ���� ���, �� �޼��带 �����մϴ�.

	// "Quit" �׼ǿ� ���� ShowReturnToMainMenu �޼��带 ���ε��մϴ�.
	InputComponent->BindAction("Quit", IE_Pressed, this, &ABlasterPlayerController::ShowReturnToMainMenu);
}

// ������ ���� �ð� ��û�� ������ �޼����Դϴ�.
void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds(); // ������ ���� �ð��� ����ϴ�.
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt); // Ŭ���̾�Ʈ���� �����մϴ�.
}

// Ŭ���̾�Ʈ���� ���� �ð��� �����ϴ� �޼����Դϴ�.
void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest; // �պ� �ð��� ����մϴ�.
	SingleTripTime = 0.5f * RoundTripTime; // �ܹ��� �ð��� ����մϴ�.
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime; // ������ ���� �ð��� ����մϴ�.
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds(); // Ŭ���̾�Ʈ�� ���� ������ �ð� ���̸� �����մϴ�.
}

// ������ ���� �ð��� ��� �޼����Դϴ�.
float ABlasterPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds(); // ������� ���� �ð��� �ٷ� ��ȯ�մϴ�.
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta; // Ŭ���̾�Ʈ��� �ð� ���̸� ���� �ð��� ��ȯ�մϴ�.
}

// �÷��̾ �޾��� �� ȣ��Ǵ� �޼����Դϴ�.
void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer(); // �θ� Ŭ������ ReceivedPlayer �޼��带 ȣ���մϴ�.
	if (IsLocalController()) // ���� ��Ʈ�ѷ��� ���
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds()); // ������ ���� �ð��� ��û�մϴ�.
	}
}

// ��ġ ���°� �����Ǿ��� �� ȣ��Ǵ� �޼����Դϴ�.
void ABlasterPlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
	MatchState = State; // ��ġ ���¸� �����մϴ�.

	// ��ġ ���¿� ���� �޼��带 ȣ���մϴ�.
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted(bTeamsMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

// ��ġ ���°� ����Ǿ��� �� ȣ��Ǵ� �޼����Դϴ�.
void ABlasterPlayerController::OnRep_MatchState()
{
	// ��ġ ���¿� ���� �޼��带 ȣ���մϴ�.
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}


// ��ġ�� ���۵Ǿ��� �� ó���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::HandleMatchHasStarted(bool bTeamsMatch) // bTeamsMatch: �� ��ġ ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
{
	if (HasAuthority()) bShowTeamScores = bTeamsMatch; // ���� ������ ������, �� ���� ǥ�� ���θ� �����մϴ�.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD; // BlasterHUD ��ü�� �ʱ�ȭ�մϴ�.

	if (BlasterHUD) // BlasterHUD�� ��ȿ�� ���
	{
		if (BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay(); // ĳ���� �������̰� ������ �߰��մϴ�.
		if (BlasterHUD->Announcement)
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden); // ���������� ����ϴ�.
		}
		if (!HasAuthority()) return; // ���� ������ ������ �޼��带 �����մϴ�.

		if (bTeamsMatch)
		{
			InitTeamScores(); // �� ������ �ʱ�ȭ�մϴ�.
		}
		else
		{
			HideTeamScores(); // �� ������ ����ϴ�.
		}
	}
}

// ��ٿ��� ó���ϴ� �޼����Դϴ�.
void ABlasterPlayerController::HandleCooldown()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD; // BlasterHUD ��ü�� �ʱ�ȭ�մϴ�.

	if (BlasterHUD) // BlasterHUD�� ��ȿ�� ���
	{
		BlasterHUD->CharacterOverlay->RemoveFromParent(); // ĳ���� �������̸� �����մϴ�.
		bool bHUDValid = BlasterHUD->Announcement &&
			BlasterHUD->Announcement->AnnouncementText &&
			BlasterHUD->Announcement->InfoText; // HUD ��ȿ���� �˻��մϴ�.

		if (bHUDValid) // HUD�� ��ȿ�� ���
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible); // ���������� ǥ���մϴ�.
			FString AnnouncementText = Announcement::NewMatchStartsIn; // �������� �ؽ�Ʈ�� �����մϴ�.
			BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText)); // �ؽ�Ʈ�� HUD�� �����մϴ�.

			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
			if (BlasterGameState && BlasterPlayerState) // ���� ���¿� �÷��̾� ���°� ��ȿ�� ���
			{
				TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers; // �ְ� ������ ����� ����ϴ�.
				FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers); // ���� �ؽ�Ʈ�� ����ϴ�.

				BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString)); // ���� �ؽ�Ʈ�� HUD�� �����մϴ�.
			}
		}
	}

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn()); // �÷��̾� ĳ���͸� ����ϴ�.
	if (BlasterCharacter && BlasterCharacter->GetCombat()) // ĳ���Ϳ� ���� ���°� ��ȿ�� ���
	{
		BlasterCharacter->bDisableGameplay = true; // �����÷��̸� ��Ȱ��ȭ�մϴ�.
		BlasterCharacter->GetCombat()->FireButtonPressed(false); // �߻� ��ư�� ��Ȱ��ȭ�մϴ�.
	}
}


// �־��� �÷��̾� ��Ͽ� ���� ���� �ؽ�Ʈ�� �������� �޼����Դϴ�.
FString ABlasterPlayerController::GetInfoText(const TArray<class ABlasterPlayerState*>& Players) // Players: ���� �ؽ�Ʈ�� ���� �÷��̾� ���� �迭�Դϴ�.
{
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>(); // ���� �÷��̾� ���¸� �����ɴϴ�.
	if (BlasterPlayerState == nullptr) return FString(); // �÷��̾� ���°� ������ �� ���ڿ��� ��ȯ�մϴ�.
	FString InfoTextString; // ��ȯ�� ���� �ؽ�Ʈ�� �����ϴ� ���ڿ��Դϴ�.

	if (Players.Num() == 0) // �÷��̾ ���� ���
	{
		InfoTextString = Announcement::ThereIsNoWinner; // ����� ���� �޽����� �����մϴ�.
	}
	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState) // ���� �÷��̾ ����� ���
	{
		InfoTextString = Announcement::YouAreTheWinner; // ��� �޽����� �����մϴ�.
	}
	else if (Players.Num() == 1) // �ٸ� �÷��̾ ����� ���
	{
		InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *Players[0]->GetPlayerName()); // ������� �̸��� ������ �޽����� �����մϴ�.
	}
	else if (Players.Num() > 1) // ���� �÷��̾ ����� ���
	{
		InfoTextString = Announcement::PlayersTiedForTheWin; // ����� ���� �޽����� �����մϴ�.
		InfoTextString.Append(FString("\n"));
		for (auto TiedPlayer : Players) // ����� �� �÷��̾ ����
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName())); // �÷��̾��� �̸��� ���� �ؽ�Ʈ�� �߰��մϴ�.
		}
	}

	return InfoTextString; // ���� �ؽ�Ʈ�� ��ȯ�մϴ�.
}

// �־��� ���� ���¿� ���� �� ���� �ؽ�Ʈ�� �������� �޼����Դϴ�.
FString ABlasterPlayerController::GetTeamsInfoText(ABlasterGameState* BlasterGameState) // BlasterGameState: �� ���� �ؽ�Ʈ�� ���� ���� �����Դϴ�.
{
	if (BlasterGameState == nullptr) return FString(); // ���� ���°� ������ �� ���ڿ��� ��ȯ�մϴ�.
	FString InfoTextString; // ��ȯ�� ���� �ؽ�Ʈ�� �����ϴ� ���ڿ��Դϴ�.

	const int32 RedTeamScore = BlasterGameState->RedTeamScore; // ���� ���� �����Դϴ�.
	const int32 BlueTeamScore = BlasterGameState->BlueTeamScore; // ��� ���� �����Դϴ�.

	if (RedTeamScore == 0 && BlueTeamScore == 0) // �� �� ��� ������ ���� ���
	{
		InfoTextString = Announcement::ThereIsNoWinner; // ����� ���� �޽����� �����մϴ�.
	}
	else if (RedTeamScore == BlueTeamScore) // �� ���� ������ ���� ���
	{
		InfoTextString = FString::Printf(TEXT("%s\n"), *Announcement::TeamsTiedForTheWin); // �� ���� �޽����� �����մϴ�.
		InfoTextString.Append(Announcement::RedTeam);
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(Announcement::BlueTeam);
		InfoTextString.Append(TEXT("\n"));
	}
	else if (RedTeamScore > BlueTeamScore) // ���� ���� �̱� ���
	{
		InfoTextString = Announcement::RedTeamWins; // ���� �� ��� �޽����� �����մϴ�.
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, RedTeamScore)); // ���� ���� ������ ���� �ؽ�Ʈ�� �߰��մϴ�.
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, BlueTeamScore)); // ��� ���� ������ ���� �ؽ�Ʈ�� �߰��մϴ�.
	}
	else if (BlueTeamScore > RedTeamScore) // ��� ���� �̱� ���
	{
		InfoTextString = Announcement::BlueTeamWins; // ��� �� ��� �޽����� �����մϴ�.
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, BlueTeamScore)); // ��� ���� ������ ���� �ؽ�Ʈ�� �߰��մϴ�.
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, RedTeamScore)); // ���� ���� ������ ���� �ؽ�Ʈ�� �߰��մϴ�.
	}

	return InfoTextString; // ���� �ؽ�Ʈ�� ��ȯ�մϴ�.
}

