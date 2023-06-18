#pragma once

#define TRACE_LENGTH 80000.f

#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

UENUM(BlueprintType)
// ������ ������ ��Ÿ���� ������
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),       // ���ݼ���
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),   // ���� �߻��
	EWT_Pistol UMETA(DisplayName = "Pistol"),                    // ����
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"),     // �������
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),                  // ��ź��
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),         // ���ݼ���
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"), // ����ź �߻��
	EWT_Flag UMETA(DisplayName = "Flag"),                        // ���

	EWT_MAX UMETA(DisplayName = "DefaultMAX")                    // �⺻ �ִ밪
};
