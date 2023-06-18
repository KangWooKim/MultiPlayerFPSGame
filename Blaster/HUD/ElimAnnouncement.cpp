// Fill out your copyright notice in the Description page of Project Settings.


#include "ElimAnnouncement.h"
#include "Components/TextBlock.h"

// UElimAnnouncement Ŭ������ Ư�� �÷��̾ �ٸ� �÷��̾ �����ߴٴ� ����� �˸��� �޽����� �����ϴ� �޼��带 ������ �ֽ��ϴ�.
void UElimAnnouncement::SetElimAnnouncementText(FString AttackerName, FString VictimName) // �޼����� ���ڷ� �����ڿ� ������� �̸��� �޽��ϴ�.
{
	// �����ڰ� ����ڸ� �����ߴٴ� ���ڿ��� �����մϴ�. 
	FString ElimAnnouncementText = FString::Printf(TEXT("%s elimmed %s!"), *AttackerName, *VictimName);

	// AnnouncementText�� null�� �ƴ� ��쿡�� �ؽ�Ʈ�� �����մϴ�.
	// �̴� �츮�� ����� �������̽� ��Ҹ� �����ϰ� �����ϰ� �ִ��� Ȯ���ϴ� ���� �߿��ϱ� �����Դϴ�.
	if (AnnouncementText)
	{
		// AnnouncementText ������ �ؽ�Ʈ�� ElimAnnouncementText�� �����մϴ�.
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}
