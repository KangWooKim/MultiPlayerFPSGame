// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "Announcement.h"
#include "ElimAnnouncement.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

// ABlasterHUD Ŭ������ BeginPlay �޼����Դϴ�. ���� �÷��̰� ���۵� �� ȣ��˴ϴ�.
void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();  // �θ� Ŭ������ BeginPlay �޼��带 ȣ���մϴ�.
}

// Attacker�� Victim�� ���������� �˸��� �޽����� ȭ�鿡 ǥ���ϴ� �޼����Դϴ�.
void ABlasterHUD::AddElimAnnouncement(FString Attacker, FString Victim)  // Attacker: ������ �̸�, Victim: ������ �̸�
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;  // OwningPlayer�� null�̸� GetOwningPlayerController�� �����ɴϴ�.
	if (OwningPlayer && ElimAnnouncementClass)  // OwningPlayer�� ElimAnnouncementClass�� ��ȿ���� Ȯ���մϴ�.
	{
		// ElimAnnouncementClass ������ ������ �����մϴ�.
		UElimAnnouncement* ElimAnnouncementWidget = CreateWidget<UElimAnnouncement>(OwningPlayer, ElimAnnouncementClass);
		if (ElimAnnouncementWidget)  // ���� ������ ���������� Ȯ���մϴ�.
		{
			// ������ �ؽ�Ʈ�� �����ϰ� ȭ�鿡 ǥ���մϴ�.
			ElimAnnouncementWidget->SetElimAnnouncementText(Attacker, Victim);
			ElimAnnouncementWidget->AddToViewport();

			// �̹� ǥ�õ� �޽����� ���� �̵��մϴ�.
			for (UElimAnnouncement* Msg : ElimMessages)
			{
				if (Msg && Msg->AnnouncementBox)
				{
					UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Msg->AnnouncementBox);
					if (CanvasSlot)
					{
						FVector2D Position = CanvasSlot->GetPosition();
						FVector2D NewPosition(
							CanvasSlot->GetPosition().X,
							Position.Y - CanvasSlot->GetSize().Y
						);
						CanvasSlot->SetPosition(NewPosition);
					}
				}
			}

			// �޽����� ��Ͽ� �߰��մϴ�.
			ElimMessages.Add(ElimAnnouncementWidget);

			// ���� �ð� �� �޽����� �����ϴ� Ÿ�̸Ӹ� �����մϴ�.
			FTimerHandle ElimMsgTimer;
			FTimerDelegate ElimMsgDelegate;
			ElimMsgDelegate.BindUFunction(this, FName("ElimAnnouncementTimerFinished"), ElimAnnouncementWidget);
			GetWorldTimerManager().SetTimer(
				ElimMsgTimer,
				ElimMsgDelegate,
				ElimAnnouncementTime,
				false
			);
		}
	}
}

// ElimAnnouncement �޽����� Ÿ�̸Ӱ� ������ �� ȣ��Ǵ� �޼����Դϴ�.
void ABlasterHUD::ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove)  // MsgToRemove: ������ �޽��� ����
{
	if (MsgToRemove)  // MsgToRemove�� ��ȿ���� Ȯ���մϴ�.
	{
		MsgToRemove->RemoveFromParent();  // ��ȿ�ϴٸ� �θ� �������κ��� �����մϴ�.
	}
}

// ĳ���� �������� ������ ȭ�鿡 �߰��ϴ� �޼����Դϴ�.
void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();  // �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
	if (PlayerController && CharacterOverlayClass)  // PlayerController�� CharacterOverlayClass�� ��ȿ���� Ȯ���մϴ�.
	{
		// CharacterOverlayClass ������ ������ �����ϰ� ȭ�鿡 ǥ���մϴ�.
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

// ABlasterHUD Ŭ������ AddAnnouncement �޼����Դϴ�. �÷��̾�� �˸��� ǥ���ϴ� ������ �����ϰ� ȭ�鿡 �߰��մϴ�.
void ABlasterHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();  // �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
	if (PlayerController && AnnouncementClass)  // PlayerController�� AnnouncementClass�� ��ȿ���� Ȯ���մϴ�.
	{
		// AnnouncementClass ������ ������ �����ϰ� ȭ�鿡 �߰��մϴ�.
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

// ȭ�鿡 HUD (Head-Up Display, ���� ���÷���)�� �׸��� �޼����Դϴ�. �̴� �÷��̾�� ������ �߿��� ������ �����ִ� ������ �մϴ�.
void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();  // �θ� Ŭ������ DrawHUD �޼��带 ȣ���մϴ�.

	FVector2D ViewportSize;  // ����Ʈ�� ũ�⸦ ������ �����Դϴ�.
	if (GEngine)  // ���� ������ ��ȿ���� Ȯ���մϴ�.
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);  // ���� ������ ���� ����Ʈ�� ũ�⸦ �����ɴϴ�.
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);  // ����Ʈ�� �߽� ��ǥ�� ����մϴ�.

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;  // �������� Ȯ���� ����մϴ�.

		// HUDPackage�� ���Ե� �� ������ �������� �׸��ϴ�.
		// �������� ��ȿ�� ��쿡�� �׸��ϴ�.
		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
}

// �������� �׸��� �޼����Դϴ�.
void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();  // �ؽ�ó�� �ʺ� �����ɴϴ�.
	const float TextureHeight = Texture->GetSizeY();  // �ؽ�ó�� ���̸� �����ɴϴ�.
	const FVector2D TextureDrawPoint(  // �ؽ�ó�� �׸� ��ġ�� ����մϴ�.
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	// �ؽ�ó�� ȭ�鿡 �׸��ϴ�.
	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
	);
}
