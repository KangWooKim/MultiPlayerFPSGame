#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),  // ����: �̻�� ��
	ECS_Reloading UMETA(DisplayName = "Reloading"),  // ����: ������ ��
	ECS_ThrowingGrenade UMETA(DisplayName = "Throwing Grenade"),  // ����: ����ź ������ ��
	ECS_SwappingWeapons UMETA(DisplayName = "Swapping Weapons"),  // ����: ���� ��ü ��

	ECS_MAX UMETA(DisplayName = "Default MAX")  // ����: �⺻ �ִ밪
};
