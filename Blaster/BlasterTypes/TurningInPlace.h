#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DisplayName = "Turning Left"),  // ȸ�� ����: ��ȸ�� ��
	ETIP_Right UMETA(DisplayName = "Turning Right"),  // ȸ�� ����: ��ȸ�� ��
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"),  // ȸ�� ����: ȸ������ ����

	ETIP_MAX UMETA(DisplayName = "Default MAX")  // ȸ�� ����: �⺻ �ִ밪
};
