// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
// HUD�� �����ϴ� ��ҵ��� ��� ����ü�Դϴ�.
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;  // �߾��� ������ �ؽ�ó�� ��Ÿ���ϴ�.
	UTexture2D* CrosshairsLeft;  // ������ ������ �ؽ�ó�� ��Ÿ���ϴ�.
	UTexture2D* CrosshairsRight;  // �������� ������ �ؽ�ó�� ��Ÿ���ϴ�.
	UTexture2D* CrosshairsTop;  // ����� ������ �ؽ�ó�� ��Ÿ���ϴ�.
	UTexture2D* CrosshairsBottom;  // �ϴ��� ������ �ؽ�ó�� ��Ÿ���ϴ�.
	float CrosshairSpread;  // �������� Ȯ�굵�� ��Ÿ���ϴ�.
	FLinearColor CrosshairsColor;  // �������� ������ ��Ÿ���ϴ�.
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;  // ȭ�鿡 HUD�� �׸��� �޼����Դϴ�.

	// �÷��̾��� ���¸� ǥ���ϴ� ������ Ŭ�����Դϴ�.
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	void AddCharacterOverlay();  // �÷��̾��� ���¸� ȭ�鿡 ǥ���ϴ� ������ �߰��ϴ� �޼����Դϴ�.

	// �÷��̾��� ���¸� ǥ���ϴ� ������ �ν��Ͻ��Դϴ�.
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	// �˸��� ǥ���ϴ� ������ Ŭ�����Դϴ�.
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	// �˸��� ǥ���ϴ� ������ �ν��Ͻ��Դϴ�.
	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnnouncement();  // �˸��� ȭ�鿡 ǥ���ϴ� ������ �߰��ϴ� �޼����Դϴ�.
	void AddElimAnnouncement(FString Attacker, FString Victim);  // Ż�� �˸��� �߰��ϴ� �޼����Դϴ�. Attacker�� Victim�� ���� �����ڿ� �����ڸ� ��Ÿ���ϴ�.
protected:
	virtual void BeginPlay() override;  // ������ ���۵� �� ȣ��Ǵ� �޼����Դϴ�.

private:
	// �÷��̾� ��Ʈ�ѷ��� �����Դϴ�. 
	UPROPERTY()
		class APlayerController* OwningPlayer;

	// HUD ��ҵ��� ��� �ִ� ��Ű���Դϴ�.
	FHUDPackage HUDPackage;

	// �ؽ�ó, ����Ʈ �߽� ��ġ, Ȯ��, ������ ���ڷ� �޾Ƽ� �������� �׸��� �޼����Դϴ�.
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	// �������� �ִ� Ȯ�� ũ���Դϴ�. �⺻���� 16�Դϴ�.
	UPROPERTY(EditAnywhere)
		float CrosshairSpreadMax = 16.f;

	// Ż�� �˸� ������ Ŭ�����Դϴ�.
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	// Ż�� �˸��� ǥ�õǴ� �ð��Դϴ�. �⺻���� 2.5���Դϴ�.
	UPROPERTY(EditAnywhere)
		float ElimAnnouncementTime = 2.5f;

	// Ż�� �˸� Ÿ�̸Ӱ� ������ �� ȣ��Ǵ� �޼����Դϴ�. ������ �޽����� ���ڷ� �޽��ϴ�.
	UFUNCTION()
		void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	// ȭ�鿡 ǥ�õ� Ż�� �˸� �޽������� �迭�Դϴ�.
	UPROPERTY()
		TArray<UElimAnnouncement*> ElimMessages;

public:
	// HUD ��Ű���� �����ϴ� �޼����Դϴ�. ������ ��Ű���� ���ڷ� �޽��ϴ�.
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }

};
