// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Flag.generated.h"

/**
 * 
 */
 // Ŭ���� ���� ��ũ��
UCLASS()
// Blaster API�� AFlag Ŭ����, AWeapon Ŭ������ ����մϴ�.
class BLASTER_API AFlag : public AWeapon
{
    // �ٵ� ���� ��ũ��
    GENERATED_BODY()

public:
    // AFlag Ŭ������ �⺻ ������
    AFlag();

    // ��ӵǾ��� ���� ������ �������ϴ� �޼���
    // �θ� Ŭ���� AWeapon�� ������ �޼��带 �������մϴ�.
    virtual void Dropped() override;

    // �÷��׸� �����ϴ� �޼���
    void ResetFlag();

protected:
    // �����Ǿ��� ���� ������ �������ϴ� �޼���
    // �θ� Ŭ���� AWeapon�� ������ �޼��带 �������մϴ�.
    virtual void OnEquipped() override;

    // ��ӵǾ��� ���� ������ �������ϴ� �޼���
    // �θ� Ŭ���� AWeapon�� ������ �޼��带 �������մϴ�.
    virtual void OnDropped() override;

    // ������ ���۵� ���� ������ �������ϴ� �޼���
    // �θ� Ŭ���� AActor�� ������ �޼��带 �������մϴ�.
    virtual void BeginPlay() override;

private:
    // �÷��� �޽��� ��Ÿ���� ������
    // Editor���� �� ������ �� �� �ֽ��ϴ�.
    UPROPERTY(VisibleAnywhere)
        UStaticMeshComponent* FlagMesh;

    // �÷����� �ʱ� ��ȯ ���� �����ϴ� ����
    FTransform InitialTransform;

public:
    // �ʱ� ��ȯ ���� �������� �޼���
    // ��ȯ�� : �÷����� �ʱ� ��ȯ��
    FORCEINLINE FTransform GetInitialTransform() const { return InitialTransform; }
};
