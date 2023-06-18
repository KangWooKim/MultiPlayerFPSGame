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

// 플레이어 컨트롤러에 대한 클래스를 정의합니다.
void ABlasterPlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	// 클라이언트로부터 공격자와 피해자에 대한 정보를 받아 처리합니다.
	ClientElimAnnouncement(Attacker, Victim);
}

// 클라이언트로부터 Elim(탈락) 알림을 구현합니다.
void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	// 현재 플레이어의 상태를 가져옵니다.
	APlayerState* Self = GetPlayerState<APlayerState>();

	// 공격자, 피해자, 그리고 현재 플레이어가 모두 존재할 경우
	if (Attacker && Victim && Self)
	{
		// BlasterHUD를 가져오거나 현재 HUD를 BlasterHUD로 캐스팅합니다.
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

		// BlasterHUD가 존재할 경우
		if (BlasterHUD)
		{
			// 현재 플레이어가 공격자이며, 피해자는 아닐 경우
			if (Attacker == Self && Victim != Self)
			{
				// 공격자와 피해자의 이름을 Elim 알림으로 추가합니다.
				BlasterHUD->AddElimAnnouncement("You", Victim->GetPlayerName());
				return;
			}
			// 현재 플레이어가 피해자이며, 공격자는 아닐 경우
			if (Victim == Self && Attacker != Self)
			{
				// 공격자와 피해자의 이름을 Elim 알림으로 추가합니다.
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "you");
				return;
			}
			// 공격자와 피해자가 동일하고, 그것이 현재 플레이어인 경우
			if (Attacker == Victim && Attacker == Self)
			{
				// 공격자와 피해자의 이름을 Elim 알림으로 추가합니다.
				BlasterHUD->AddElimAnnouncement("You", "yourself");
				return;
			}
			// 공격자와 피해자가 동일하나, 그것이 현재 플레이어는 아닌 경우
			if (Attacker == Victim && Attacker != Self)
			{
				// 공격자와 피해자의 이름을 Elim 알림으로 추가합니다.
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "themselves");
				return;
			}

			// 그 외의 경우, 공격자와 피해자의 이름을 Elim 알림으로 추가합니다.
			BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), Victim->GetPlayerName());
		}
	}
}

// 플레이 시작 시 실행되는 함수입니다.
void ABlasterPlayerController::BeginPlay()
{
	// 부모 클래스의 BeginPlay 함수를 실행합니다.
	Super::BeginPlay();

	// 현재 HUD를 BlasterHUD로 캐스팅합니다.
	BlasterHUD = Cast<ABlasterHUD>(GetHUD());

	// 서버의 매치 상태를 확인합니다.
	ServerCheckMatchState();
}

// 리플리케이션에 필요한 속성들을 설정합니다.
void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 상위 클래스의 동일 함수를 호출합니다.
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// MatchState와 bShowTeamScores 변수를 리플리케이션에 포함시킵니다.
	DOREPLIFETIME(ABlasterPlayerController, MatchState);
	DOREPLIFETIME(ABlasterPlayerController, bShowTeamScores);
}

// 팀 점수를 숨기는 메서드입니다.
void ABlasterPlayerController::HideTeamScores()
{
	// BlasterHUD를 가져오거나 현재 HUD를 BlasterHUD로 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD의 유효성을 검사합니다.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore &&
		BlasterHUD->CharacterOverlay->ScoreSpacerText;

	// HUD가 유효할 경우, 팀 점수를 초기화합니다.
	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->ScoreSpacerText->SetText(FText());
	}
}

// 팀 점수를 초기화하는 메서드입니다.
void ABlasterPlayerController::InitTeamScores()
{
	// BlasterHUD를 가져오거나 현재 HUD를 BlasterHUD로 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD의 유효성을 검사합니다.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore &&
		BlasterHUD->CharacterOverlay->ScoreSpacerText;

	// HUD가 유효할 경우, 팀 점수를 초기화합니다.
	if (bHUDValid)
	{
		FString Zero("0");
		FString Spacer("|");
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->ScoreSpacerText->SetText(FText::FromString(Spacer));
	}
}


// HUD에 빨간팀 점수를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
	// BlasterHUD를 가져오거나 현재 HUD를 BlasterHUD로 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD의 유효성을 검사합니다.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore;

	// HUD가 유효할 경우, 빨간팀 점수를 설정합니다.
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), RedScore);
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(ScoreText));
	}
}

// HUD에 파란팀 점수를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
	// BlasterHUD를 가져오거나 현재 HUD를 BlasterHUD로 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// HUD의 유효성을 검사합니다.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->BlueTeamScore;

	// HUD가 유효할 경우, 파란팀 점수를 설정합니다.
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(ScoreText));
	}
}

// 매 틱마다 실행되는 메서드입니다.
void ABlasterPlayerController::Tick(float DeltaTime)
{
	// 상위 클래스의 Tick 메서드를 호출합니다.
	Super::Tick(DeltaTime);

	// HUD에 시간을 설정하고, 시간 동기화를 확인합니다.
	SetHUDTime();
	CheckTimeSync(DeltaTime);

	// 초기화 상태를 확인합니다.
	PollInit();

	// 네트워크 지연시간을 확인합니다.
	CheckPing(DeltaTime);
}


// 네트워크 지연시간 (핑)을 확인하는 메서드입니다.
void ABlasterPlayerController::CheckPing(float DeltaTime)
{
	// 서버 권한이 있는 경우 이 메서드를 종료합니다.
	if (HasAuthority()) return;

	// 핑 체크를 위한 누적 시간을 증가시킵니다.
	HighPingRunningTime += DeltaTime;

	// 핑 체크 주기가 되었는지 확인합니다.
	if (HighPingRunningTime > CheckPingFrequency)
	{
		// PlayerState를 가져오거나 현재 PlayerState를 캐스팅합니다.
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState;

		// PlayerState가 유효한 경우, 핑을 확인합니다.
		if (PlayerState)
		{
			// 핑이 너무 높은 경우 경고를 출력하고, 상태를 서버에 보고합니다.
			if (PlayerState->GetPing() * 4 > HighPingThreshold) // 핑은 압축되어 있습니다. 실제 핑은 핑 / 4 입니다.
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				// 핑이 문제 없는 경우, 상태를 서버에 보고합니다.
				ServerReportPingStatus(false);
			}
		}

		// 핑 체크를 위한 누적 시간을 초기화합니다.
		HighPingRunningTime = 0.f;
	}

	// 핑 경고 애니메이션 재생 상태를 확인합니다.
	bool bHighPingAnimationPlaying =
		BlasterHUD && BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingAnimation &&
		BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation);

	// 핑 경고 애니메이션 재생 중인 경우, 지속 시간을 검사하여 종료합니다.
	if (bHighPingAnimationPlaying)
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

// 메인 메뉴로 돌아가는 옵션을 보여주는 메서드입니다.
void ABlasterPlayerController::ShowReturnToMainMenu()
{
	// 메인 메뉴 위젯이 없는 경우 이 메서드를 종료합니다.
	if (ReturnToMainMenuWidget == nullptr) return;

	// 메인 메뉴를 생성하거나 존재하는 메뉴를 가져옵니다.
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}

	// 메인 메뉴가 유효한 경우, 메뉴를 표시하거나 제거합니다.
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


// 팀 점수 표시 상태를 갱신하는 메서드입니다.
void ABlasterPlayerController::OnRep_ShowTeamScores()
{
	// bShowTeamScores 값에 따라 팀 점수를 초기화하거나 숨깁니다.
	if (bShowTeamScores)
	{
		InitTeamScores();  // 팀 점수를 초기화합니다.
	}
	else
	{
		HideTeamScores();  // 팀 점수를 숨깁니다.
	}
}

// 핑이 너무 높은지 서버에 보고하는 메서드입니다.
void ABlasterPlayerController::ServerReportPingStatus_Implementation(bool bHighPing)  // bHighPing: 핑이 높은지 낮은지를 나타내는 불린값입니다.
{
	HighPingDelegate.Broadcast(bHighPing);  // 높은 핑 상태를 브로드캐스트합니다.
}

// 서버와의 시간 동기화를 확인하는 메서드입니다.
void ABlasterPlayerController::CheckTimeSync(float DeltaTime)  // DeltaTime: 이전 프레임에서 현재 프레임까지 걸린 시간입니다.
{
	TimeSyncRunningTime += DeltaTime;

	// 로컬 컨트롤러이고, 동기화 주기가 경과했으면, 서버에 현재 시간을 요청합니다.
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;  // 동기화를 위한 누적 시간을 초기화합니다.
	}
}

// 높은 핑 경고를 표시하는 메서드입니다.
void ABlasterPlayerController::HighPingWarning()
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	// BlasterHUD가 유효하면, 높은 핑 애니메이션을 표시합니다.
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(1.f);  // 핑 이미지의 불투명도를 설정합니다.

		// 핑 애니메이션을 재생합니다.
		BlasterHUD->CharacterOverlay->PlayAnimation(
			BlasterHUD->CharacterOverlay->HighPingAnimation,
			0.f,
			5);
	}
}


// 높은 핑 경고를 중지하는 메서드입니다.
void ABlasterPlayerController::StopHighPingWarning()
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);  // 핑 이미지의 불투명도를 0으로 설정합니다.

		if (BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation))
		{
			// 현재 핑 애니메이션이 재생 중이면 중지합니다.
			BlasterHUD->CharacterOverlay->StopAnimation(BlasterHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}

// 게임 모드의 상태를 체크하는 메서드입니다.
void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));  // 현재 게임 모드를 가져옵니다.

	if (GameMode)
	{
		// 게임 모드로부터 각각의 상태 값을 가져와 저장합니다.
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();

		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

// 클라이언트가 중간에 게임에 참가하는 경우를 처리하는 메서드입니다.
void ABlasterPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	// 각 매개변수 값을 저장합니다.
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;

	OnMatchStateSet(MatchState);  // 게임 상태를 설정합니다.

	if (BlasterHUD && MatchState == MatchState::WaitingToStart)
	{
		BlasterHUD->AddAnnouncement();  // 게임이 시작 대기 중이면 알림을 추가합니다.
	}
}

// 새로운 Pawn을 점유할 때 호출되는 메서드입니다.
void ABlasterPlayerController::OnPossess(APawn* InPawn)  // InPawn: 점유할 새로운 Pawn입니다.
{
	Super::OnPossess(InPawn);  // 부모 클래스의 OnPossess 메서드를 호출합니다.

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn);  // InPawn을 ABlasterCharacter로 캐스팅합니다.

	if (BlasterCharacter)
	{
		SetHUDHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());  // HUD의 체력을 설정합니다.
	}
}


// HUD의 체력을 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth) // Health: 설정할 체력 값, MaxHealth: 체력의 최대 값
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HealthBar &&
		BlasterHUD->CharacterOverlay->HealthText;

	if (bHUDValid)
	{
		// 체력의 퍼센트를 계산하고, HealthBar와 HealthText를 업데이트합니다.
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		// 만약 HUD가 초기화되지 않았다면, 초기화를 해야하는 것으로 표시하고 체력 값을 저장합니다.
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

// HUD의 방어막을 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDShield(float Shield, float MaxShield) // Shield: 설정할 방어막 값, MaxShield: 방어막의 최대 값
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ShieldBar &&
		BlasterHUD->CharacterOverlay->ShieldText;

	if (bHUDValid)
	{
		// 방어막의 퍼센트를 계산하고, ShieldBar와 ShieldText를 업데이트합니다.
		const float ShieldPercent = Shield / MaxShield;
		BlasterHUD->CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		FString ShieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		BlasterHUD->CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
	}
	else
	{
		// 만약 HUD가 초기화되지 않았다면, 초기화를 해야하는 것으로 표시하고 방어막 값을 저장합니다.
		bInitializeShield = true;
		HUDShield = Shield;
		HUDMaxShield = MaxShield;
	}
}

// HUD의 점수를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDScore(float Score) // Score: 설정할 점수 값
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ScoreAmount;

	if (bHUDValid)
	{
		// ScoreAmount를 업데이트합니다.
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		// 만약 HUD가 초기화되지 않았다면, 초기화를 해야하는 것으로 표시하고 점수 값을 저장합니다.
		bInitializeScore = true;
		HUDScore = Score;
	}
}


// HUD에 Defeats 정보를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDDefeats(int32 Defeats) // Defeats: 설정할 Defeats 값
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->DefeatsAmount;

	if (bHUDValid)
	{
		// DefeatsAmount를 업데이트합니다.
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		BlasterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		// 만약 HUD가 초기화되지 않았다면, 초기화를 해야하는 것으로 표시하고 Defeats 값을 저장합니다.
		bInitializeDefeats = true;
		HUDDefeats = Defeats;
	}
}

// HUD에 무기의 탄약을 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDWeaponAmmo(int32 Ammo) // Ammo: 설정할 무기의 탄약 값
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount;

	if (bHUDValid)
	{
		// WeaponAmmoAmount를 업데이트합니다.
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		// 만약 HUD가 초기화되지 않았다면, 초기화를 해야하는 것으로 표시하고 무기의 탄약 값을 저장합니다.
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

// HUD에 보유한 탄약을 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo) // Ammo: 설정할 보유한 탄약 값
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount;

	if (bHUDValid)
	{
		// CarriedAmmoAmount를 업데이트합니다.
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		// 만약 HUD가 초기화되지 않았다면, 초기화를 해야하는 것으로 표시하고 보유한 탄약 값을 저장합니다.
		bInitializeCarriedAmmo = true;
		HUDCarriedAmmo = Ammo;
	}
}

// HUD에 경기 카운트다운 정보를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime) // CountdownTime: 설정할 카운트다운 시간(초)
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->MatchCountdownText;

	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			// 카운트다운 시간이 0 미만일 경우 텍스트를 비웁니다.
			BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}

		// 분과 초로 변환합니다.
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		// 카운트다운 텍스트를 업데이트합니다.
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

// HUD에 공지 카운트다운 정보를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime) // CountdownTime: 설정할 카운트다운 시간(초)
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->Announcement &&
		BlasterHUD->Announcement->WarmupTime;

	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			// 카운트다운 시간이 0 미만일 경우 텍스트를 비웁니다.
			BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		// 분과 초로 변환합니다.
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		// 카운트다운 텍스트를 업데이트합니다.
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

// HUD에 수류탄 수를 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDGrenades(int32 Grenades) // Grenades: 설정할 수류탄의 수
{
	// BlasterHUD를 가져오거나 현재 HUD를 캐스팅합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->GrenadesText;

	if (bHUDValid)
	{
		// 수류탄의 수를 텍스트로 변환합니다.
		FString GrenadesText = FString::Printf(TEXT("%d"), Grenades);
		BlasterHUD->CharacterOverlay->GrenadesText->SetText(FText::FromString(GrenadesText));
	}
	else
	{
		// 수류탄의 수를 초기화합니다.
		bInitializeGrenades = true;
		HUDGrenades = Grenades;
	}
}


// HUD에 남은 시간을 설정하는 메서드입니다.
void ABlasterPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;

	// 현재 게임 상태에 따라 남은 시간을 계산합니다.
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft); // 남은 초로 변환합니다.

	if (HasAuthority()) // 서버에서 실행되는 경우
	{
		if (BlasterGameMode == nullptr)
		{
			// 게임 모드를 가져오거나 현재 게임 모드를 캐스팅합니다.
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
			// 공지 카운트다운을 설정합니다.
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			// 경기 카운트다운을 설정합니다.
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

// 초기화 요청을 확인하는 메서드입니다.
void ABlasterPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (BlasterHUD && BlasterHUD->CharacterOverlay)
		{
			CharacterOverlay = BlasterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				// 각 요소가 초기화되어야 하는 경우 해당 요소의 HUD를 설정합니다.
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


// 입력 구성요소를 설정하는 메서드입니다.
void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); // 부모 클래스의 SetupInputComponent 메서드를 호출합니다.
	if (InputComponent == nullptr) return; // 입력 구성요소가 없는 경우, 이 메서드를 종료합니다.

	// "Quit" 액션에 대해 ShowReturnToMainMenu 메서드를 바인딩합니다.
	InputComponent->BindAction("Quit", IE_Pressed, this, &ABlasterPlayerController::ShowReturnToMainMenu);
}

// 서버에 서버 시간 요청을 보내는 메서드입니다.
void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds(); // 서버의 현재 시간을 얻습니다.
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt); // 클라이언트에게 보고합니다.
}

// 클라이언트에게 서버 시간을 보고하는 메서드입니다.
void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest; // 왕복 시간을 계산합니다.
	SingleTripTime = 0.5f * RoundTripTime; // 단방향 시간을 계산합니다.
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime; // 서버의 현재 시간을 계산합니다.
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds(); // 클라이언트와 서버 사이의 시간 차이를 저장합니다.
}

// 서버의 현재 시간을 얻는 메서드입니다.
float ABlasterPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds(); // 서버라면 현재 시간을 바로 반환합니다.
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta; // 클라이언트라면 시간 차이를 더한 시간을 반환합니다.
}

// 플레이어를 받았을 때 호출되는 메서드입니다.
void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer(); // 부모 클래스의 ReceivedPlayer 메서드를 호출합니다.
	if (IsLocalController()) // 로컬 컨트롤러인 경우
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds()); // 서버에 현재 시간을 요청합니다.
	}
}

// 매치 상태가 설정되었을 때 호출되는 메서드입니다.
void ABlasterPlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
	MatchState = State; // 매치 상태를 저장합니다.

	// 매치 상태에 따라 메서드를 호출합니다.
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted(bTeamsMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

// 매치 상태가 변경되었을 때 호출되는 메서드입니다.
void ABlasterPlayerController::OnRep_MatchState()
{
	// 매치 상태에 따라 메서드를 호출합니다.
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}


// 매치가 시작되었을 때 처리하는 메서드입니다.
void ABlasterPlayerController::HandleMatchHasStarted(bool bTeamsMatch) // bTeamsMatch: 팀 매치 여부를 나타내는 불리언 값입니다.
{
	if (HasAuthority()) bShowTeamScores = bTeamsMatch; // 서버 권한이 있으면, 팀 점수 표시 여부를 설정합니다.
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD; // BlasterHUD 객체를 초기화합니다.

	if (BlasterHUD) // BlasterHUD가 유효한 경우
	{
		if (BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay(); // 캐릭터 오버레이가 없으면 추가합니다.
		if (BlasterHUD->Announcement)
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden); // 공지사항을 숨깁니다.
		}
		if (!HasAuthority()) return; // 서버 권한이 없으면 메서드를 종료합니다.

		if (bTeamsMatch)
		{
			InitTeamScores(); // 팀 점수를 초기화합니다.
		}
		else
		{
			HideTeamScores(); // 팀 점수를 숨깁니다.
		}
	}
}

// 쿨다운을 처리하는 메서드입니다.
void ABlasterPlayerController::HandleCooldown()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD; // BlasterHUD 객체를 초기화합니다.

	if (BlasterHUD) // BlasterHUD가 유효한 경우
	{
		BlasterHUD->CharacterOverlay->RemoveFromParent(); // 캐릭터 오버레이를 제거합니다.
		bool bHUDValid = BlasterHUD->Announcement &&
			BlasterHUD->Announcement->AnnouncementText &&
			BlasterHUD->Announcement->InfoText; // HUD 유효성을 검사합니다.

		if (bHUDValid) // HUD가 유효한 경우
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible); // 공지사항을 표시합니다.
			FString AnnouncementText = Announcement::NewMatchStartsIn; // 공지사항 텍스트를 설정합니다.
			BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText)); // 텍스트를 HUD에 설정합니다.

			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
			if (BlasterGameState && BlasterPlayerState) // 게임 상태와 플레이어 상태가 유효한 경우
			{
				TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers; // 최고 득점자 목록을 얻습니다.
				FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers); // 정보 텍스트를 얻습니다.

				BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString)); // 정보 텍스트를 HUD에 설정합니다.
			}
		}
	}

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn()); // 플레이어 캐릭터를 얻습니다.
	if (BlasterCharacter && BlasterCharacter->GetCombat()) // 캐릭터와 전투 상태가 유효한 경우
	{
		BlasterCharacter->bDisableGameplay = true; // 게임플레이를 비활성화합니다.
		BlasterCharacter->GetCombat()->FireButtonPressed(false); // 발사 버튼을 비활성화합니다.
	}
}


// 주어진 플레이어 목록에 대한 정보 텍스트를 가져오는 메서드입니다.
FString ABlasterPlayerController::GetInfoText(const TArray<class ABlasterPlayerState*>& Players) // Players: 정보 텍스트를 얻어올 플레이어 상태 배열입니다.
{
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>(); // 현재 플레이어 상태를 가져옵니다.
	if (BlasterPlayerState == nullptr) return FString(); // 플레이어 상태가 없으면 빈 문자열을 반환합니다.
	FString InfoTextString; // 반환할 정보 텍스트를 저장하는 문자열입니다.

	if (Players.Num() == 0) // 플레이어가 없는 경우
	{
		InfoTextString = Announcement::ThereIsNoWinner; // 우승자 없음 메시지를 설정합니다.
	}
	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState) // 현재 플레이어가 우승한 경우
	{
		InfoTextString = Announcement::YouAreTheWinner; // 우승 메시지를 설정합니다.
	}
	else if (Players.Num() == 1) // 다른 플레이어가 우승한 경우
	{
		InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *Players[0]->GetPlayerName()); // 우승자의 이름을 포함한 메시지를 설정합니다.
	}
	else if (Players.Num() > 1) // 여러 플레이어가 우승한 경우
	{
		InfoTextString = Announcement::PlayersTiedForTheWin; // 우승자 동점 메시지를 설정합니다.
		InfoTextString.Append(FString("\n"));
		for (auto TiedPlayer : Players) // 우승한 각 플레이어에 대해
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName())); // 플레이어의 이름을 정보 텍스트에 추가합니다.
		}
	}

	return InfoTextString; // 정보 텍스트를 반환합니다.
}

// 주어진 게임 상태에 대한 팀 정보 텍스트를 가져오는 메서드입니다.
FString ABlasterPlayerController::GetTeamsInfoText(ABlasterGameState* BlasterGameState) // BlasterGameState: 팀 정보 텍스트를 얻어올 게임 상태입니다.
{
	if (BlasterGameState == nullptr) return FString(); // 게임 상태가 없으면 빈 문자열을 반환합니다.
	FString InfoTextString; // 반환할 정보 텍스트를 저장하는 문자열입니다.

	const int32 RedTeamScore = BlasterGameState->RedTeamScore; // 레드 팀의 점수입니다.
	const int32 BlueTeamScore = BlasterGameState->BlueTeamScore; // 블루 팀의 점수입니다.

	if (RedTeamScore == 0 && BlueTeamScore == 0) // 두 팀 모두 점수가 없는 경우
	{
		InfoTextString = Announcement::ThereIsNoWinner; // 우승자 없음 메시지를 설정합니다.
	}
	else if (RedTeamScore == BlueTeamScore) // 두 팀의 점수가 같은 경우
	{
		InfoTextString = FString::Printf(TEXT("%s\n"), *Announcement::TeamsTiedForTheWin); // 팀 동점 메시지를 설정합니다.
		InfoTextString.Append(Announcement::RedTeam);
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(Announcement::BlueTeam);
		InfoTextString.Append(TEXT("\n"));
	}
	else if (RedTeamScore > BlueTeamScore) // 레드 팀이 이긴 경우
	{
		InfoTextString = Announcement::RedTeamWins; // 레드 팀 우승 메시지를 설정합니다.
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, RedTeamScore)); // 레드 팀의 점수를 정보 텍스트에 추가합니다.
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, BlueTeamScore)); // 블루 팀의 점수를 정보 텍스트에 추가합니다.
	}
	else if (BlueTeamScore > RedTeamScore) // 블루 팀이 이긴 경우
	{
		InfoTextString = Announcement::BlueTeamWins; // 블루 팀 우승 메시지를 설정합니다.
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, BlueTeamScore)); // 블루 팀의 점수를 정보 텍스트에 추가합니다.
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, RedTeamScore)); // 레드 팀의 점수를 정보 텍스트에 추가합니다.
	}

	return InfoTextString; // 정보 텍스트를 반환합니다.
}

