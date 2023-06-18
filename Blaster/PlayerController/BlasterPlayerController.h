// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

// ���� �� ���¸� ���� ��������Ʈ �����Դϴ�.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

// ������ �÷��̾� ��Ʈ�ѷ� Ŭ���� �����Դϴ�.
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// HUD�� �÷��̾��� ü���� �����ϴ� �޼����Դϴ�. Health: ���� ü��, MaxHealth: �ִ� ü��
	void SetHUDHealth(float Health, float MaxHealth);

	// HUD�� �÷��̾��� ���� �����ϴ� �޼����Դϴ�. Shield: ���� ��, MaxShield: �ִ� ��
	void SetHUDShield(float Shield, float MaxShield);

	// HUD�� �÷��̾��� ������ �����ϴ� �޼����Դϴ�. Score: ���� ����
	void SetHUDScore(float Score);

	// HUD�� �÷��̾��� �й� Ƚ���� �����ϴ� �޼����Դϴ�. Defeats: �й� Ƚ��
	void SetHUDDefeats(int32 Defeats);

	// HUD�� �÷��̾��� ���� ���� ź���� �����ϴ� �޼����Դϴ�. Ammo: ���� ź�� ��
	void SetHUDWeaponAmmo(int32 Ammo);

	// HUD�� �÷��̾ ������ ź���� �����ϴ� �޼����Դϴ�. Ammo: ������ ź�� ��
	void SetHUDCarriedAmmo(int32 Ammo);

	// HUD�� ��ġ ī��Ʈ�ٿ��� �����ϴ� �޼����Դϴ�. CountdownTime: ���� �ð�
	void SetHUDMatchCountdown(float CountdownTime);

	// HUD�� �˸� ī��Ʈ�ٿ��� �����ϴ� �޼����Դϴ�. CountdownTime: ���� �ð�
	void SetHUDAnnouncementCountdown(float CountdownTime);

	// HUD�� ����ź ���� �����ϴ� �޼����Դϴ�. Grenades: ����ź ��
	void SetHUDGrenades(int32 Grenades);

	// ĳ���͸� �����ϸ� ȣ��Ǵ� �޼����Դϴ�. InPawn: ������ ĳ����
	virtual void OnPossess(APawn* InPawn) override;

	// �� ƽ���� ȣ��Ǵ� �޼����Դϴ�. DeltaTime: ���� ƽ ������ �ð�
	virtual void Tick(float DeltaTime) override;

	// ���� ���� �����Ǵ� �Ӽ����� �����ɴϴ�. OutLifetimeProps: ���� ���� �����Ǵ� �Ӽ���
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// �� ������ ����� �޼����Դϴ�.
	void HideTeamScores();

	// �� ������ �ʱ�ȭ�ϴ� �޼����Դϴ�.
	void InitTeamScores();

	// HUD�� ���� ���� ������ �����ϴ� �޼����Դϴ�. RedScore: ���� �� ����
	void SetHUDRedTeamScore(int32 RedScore);

	// HUD�� ��� ���� ������ �����ϴ� �޼����Դϴ�. BlueScore: ��� �� ����
	void SetHUDBlueTeamScore(int32 BlueScore);

	// ���� �ð��� ����ȭ�ؼ� �������� �޼����Դϴ�.
	virtual float GetServerTime();

	// ������ ���� ���� Ŭ���� ����ȭ�ϴ� �޼����Դϴ�.
	virtual void ReceivedPlayer() override;

	// ��ġ ���°� �����Ǹ� ȣ��Ǵ� �޼����Դϴ�. State: ��ġ ����, bTeamsMatch: �� ��ġ���� �ƴ���
	void OnMatchStateSet(FName State, bool bTeamsMatch = false);

	// ��ġ�� ���۵Ǹ� ȣ��Ǵ� �޼����Դϴ�. bTeamsMatch: �� ��ġ���� �ƴ���
	void HandleMatchHasStarted(bool bTeamsMatch = false);

	// ��ٿ� ó���ϴ� �޼����Դϴ�.
	void HandleCooldown();

	// ���� ���� �ð��Դϴ�.
	float SingleTripTime = 0.f;

	// ���� �� ��������Ʈ�Դϴ�.
	FHighPingDelegate HighPingDelegate;

	// ü�� �̺�Ʈ�� ����ϴ� �޼����Դϴ�. Attacker: ������, Victim: ������
	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);
protected:
    // ������ ���۵� �� ȣ��Ǵ� �޼����Դϴ�.
    virtual void BeginPlay() override;

    // HUD�� �ð��� �����ϴ� �޼����Դϴ�.
    void SetHUDTime();

    // �ʱ�ȭ ������ �����ϴ� �޼����Դϴ�.
    void PollInit();

    // �Է� ������Ʈ�� �����ϴ� �޼����Դϴ�.
    virtual void SetupInputComponent() override;

    // Ŭ���̾�Ʈ�� ���� ���� �ð��� ����ȭ�ϴ� �޼����Դϴ�.
    // ���� ���� �ð��� ��û�ϰ�, Ŭ���̾�Ʈ�� ��û �ð��� �����մϴ�.
	// @param TimeOfClientRequest: Ŭ���̾�Ʈ�� ��û�� ������ ���� Ŭ���̾�Ʈ�� �ð��Դϴ�.
    UFUNCTION(Server, Reliable)
    void ServerRequestServerTime(float TimeOfClientRequest);

    // ServerRequestServerTime�� ���� �������� ���� ���� �ð��� Ŭ���̾�Ʈ���� �����ϴ� �޼����Դϴ�.
	// @param TimeOfClientRequest: Ŭ���̾�Ʈ�� ��û�� ������ ���� Ŭ���̾�Ʈ�� �ð��Դϴ�.
	// @param TimeServerReceivedClientRequest: Ŭ���̾�Ʈ ��û�� ������ �޾��� ���� ���� �ð��Դϴ�.
    UFUNCTION(Client, Reliable)
    void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

    // Ŭ���̾�Ʈ�� ���� �ð��� �����Դϴ�.
    float ClientServerDelta = 0.f;

    // �ð� ����ȭ �ֱ��Դϴ�.
    UPROPERTY(EditAnywhere, Category = Time)
    float TimeSyncFrequency = 5.f;

    // �ð� ����ȭ�� ���� ���� �ð��Դϴ�.
    float TimeSyncRunningTime = 0.f;

    // �ð� ����ȭ�� Ȯ���ϴ� �޼����Դϴ�.
	// @param DeltaTime: ���� ������ ������ ��� �ð��Դϴ�.
    void CheckTimeSync(float DeltaTime);

    // ��ġ ���¸� Ȯ���ϴ� �޼����Դϴ�.
    UFUNCTION(Server, Reliable)
    void ServerCheckMatchState();

    // ���� �߰��� ������ �� ȣ��Ǵ� �޼����Դϴ�.
	// @param StateOfMatch: ���� ��ġ�� �����Դϴ�.
	// @param Warmup: ���� �ð��Դϴ�.
	// @param Match: ��ġ �ð��Դϴ�.
	// @param Cooldown: ��ٿ� �ð��Դϴ�.
	// @param StartingTime: ���� �ð��Դϴ�.
    UFUNCTION(Client, Reliable)
    void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

    // ���� �� ��� �����ִ� �޼����Դϴ�.
    void HighPingWarning();

    // ���� �� ��� �����ϴ� �޼����Դϴ�.
    void StopHighPingWarning();

    // ���� Ȯ���ϴ� �޼����Դϴ�.
	// @param DeltaTime: ���� ������ ������ ��� �ð��Դϴ�.
    void CheckPing(float DeltaTime);

    // ���� �޴��� ���ư��⸦ �����ִ� �޼����Դϴ�.
    void ShowReturnToMainMenu();

    // ü�� �˸��� Ŭ���̾�Ʈ���� �����ϴ� �޼����Դϴ�.
	// @param Attacker: �������� �÷��̾� �����Դϴ�.
	// @param Victim: �������� �÷��̾� �����Դϴ�.
    UFUNCTION(Client, Reliable)
    void ClientElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

    // �� ������ �������� �����Դϴ�.
    UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
    bool bShowTeamScores = false;

    // �� ���� ǥ�� ���� �� ȣ��Ǵ� �޼����Դϴ�.
    UFUNCTION()
    void OnRep_ShowTeamScores();

    // �÷��̾�鿡 ���� ���� �ؽ�Ʈ�� �������� �޼����Դϴ�.
	// @param Players: ������ ����� �ϴ� �÷��̾���� �迭�Դϴ�.
    FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players);

    // �� ���� �ؽ�Ʈ�� �������� �޼����Դϴ�.
	// @param BlasterGameState: ���� ���� �����Դϴ�.
    FString GetTeamsInfoText(class ABlasterGameState* BlasterGameState);

private:
    // ���� �÷��̾��� HUD�� ����ŵ�ϴ�.
    UPROPERTY()
        class ABlasterHUD* BlasterHUD;

    // ���� ���� ��带 ����ŵ�ϴ�.
    UPROPERTY()
        class ABlasterGameMode* BlasterGameMode;

    // ���� �޴��� ���ư��� ������ �����մϴ�.
    UPROPERTY(EditAnywhere, Category = HUD)
        TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

    // ���� �޴��� ���ư��� ���� Ŭ������ ����ŵ�ϴ�.
    UPROPERTY()
        class UReturnToMainMenu* ReturnToMainMenu;

    // ���� �޴��� ���ư������� ���¸� ��Ÿ���ϴ�.
    bool bReturnToMainMenuOpen = false;

    // ������ ���� �ð�, ��ġ �ð�, ���� �ð�, ��ٿ� �ð��� �����մϴ�.
    float LevelStartingTime = 0.f;
    float MatchTime = 0.f;
    float WarmupTime = 0.f;
    float CooldownTime = 0.f;
    uint32 CountdownInt = 0;

    // ��ġ�� ���¸� ��Ÿ���ϴ�.
    UPROPERTY(ReplicatedUsing = OnRep_MatchState)
        FName MatchState;

    // ���°� ����� �� ����� �Լ��Դϴ�.
    UFUNCTION()
        void OnRep_MatchState();

    // ĳ���� �������̸� ����ŵ�ϴ�.
    UPROPERTY()
        class UCharacterOverlay* CharacterOverlay;

    // HUD���� ü��, ����, �й�, ����ź, ����, ź�� ���� ��Ÿ���� �������Դϴ�.
    float HUDHealth;
    bool bInitializeHealth = false;
    float HUDMaxHealth;
    float HUDScore;
    bool bInitializeScore = false;
    int32 HUDDefeats;
    bool bInitializeDefeats = false;
    int32 HUDGrenades;
    bool bInitializeGrenades = false;
    float HUDShield;
    bool bInitializeShield = false;
    float HUDMaxShield;
    float HUDCarriedAmmo;
    bool bInitializeCarriedAmmo = false;
    float HUDWeaponAmmo;
    bool bInitializeWeaponAmmo = false;

    // ���� ���� �����ϱ� ���� �������Դϴ�.
    float HighPingRunningTime = 0.f;
    UPROPERTY(EditAnywhere)
        float HighPingDuration = 5.f;
    float PingAnimationRunningTime = 0.f;
    UPROPERTY(EditAnywhere)
        float CheckPingFrequency = 20.f;

    // ������ ���� �� ���¸� �����ϴ� �Լ��Դϴ�.
    UFUNCTION(Server, Reliable)
        void ServerReportPingStatus(bool bHighPing);

    // ���� ���� �����ϴ� �Ӱ谪�Դϴ�.
    UPROPERTY(EditAnywhere)
        float HighPingThreshold = 50.f;

};
