# BuffComponent.h

1. UBuffComponent::Heal(float HealAmount, float HealingTime): 이 함수는 주어진 시간 동안 캐릭터의 체력을 점진적으로 회복하는 버프를 설정합니다.

2. UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime): 이 함수는 주어진 시간 동안 캐릭터의 방패를 점진적으로 회복하는 버프를 설정합니다.

3. UBuffComponent::HealRampUp(float DeltaTime): 이 함수는 "Heal" 버프를 실행하는 메서드입니다. 이는 프레임당 호출되어 캐릭터의 체력을 점진적으로 회복합니다.

4. UBuffComponent::ShieldRampUp(float DeltaTime): 이 함수는 "ReplenishShield" 버프를 실행하는 메서드입니다. 이는 프레임당 호출되어 캐릭터의 방패를 점진적으로 회복합니다.

5. UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed): 이 함수는 캐릭터의 원래 이동 속도와 웅크린 상태에서의 이동 속도를 설정합니다.

6. UBuffComponent::SetInitialJumpVelocity(float Velocity): 이 함수는 캐릭터의 원래 점프 속도를 설정합니다.

7. UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime): 이 함수는 주어진 시간 동안 캐릭터의 이동 속도와 웅크린 상태에서의 이동 속도를 일시적으로 증가시킵니다.

8. UBuffComponent::ResetSpeeds(): 이 함수는 "BuffSpeed" 버프가 끝난 후 캐릭터의 이동 속도를 원래대로 되돌립니다.

9. UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime): 이 함수는 주어진 시간 동안 캐릭터의 점프 속도를 일시적으로 증가시킵니다.

10. UBuffComponent::ResetJump(): 이 함수는 "BuffJump" 버프가 끝난 후 캐릭터의 점프 속도를 원래대로 되돌립니다.

11. UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction): 이 함수는 매 프레임마다 호출되며, 이때 HealRampUp 및 ShieldRampUp 함수를 호출하여 체력 및 방패 회복 버프를 적용합니다.

이 클래스는 게임 캐릭터의 능력치를 임시적으로 증가시키는데 사용됩니다.

---

# CombatComponent.h

UCombatComponent::UCombatComponent() : 이것은 생성자입니다. 이 메서드는 UCombatComponent 클래스의 새 인스턴스를 초기화합니다. 여기서는 BaseWalkSpeed와 AimWalkSpeed을 초기화합니다.

UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const : 이 메서드는 네트워크 레플리케이션 시스템에 이 컴포넌트의 레플리케이션 속성을 알리는 데 사용됩니다. 레플리케이션은 네트워크 멀티플레이어 게임에서 중요한 개념입니다. 이 메서드는 서버에서 클라이언트로 상태를 동기화하는데 사용됩니다.

UCombatComponent::ShotgunShellReload() : 캐릭터가 권한이 있을 경우, 샷건 탄약 값을 업데이트하는 메서드입니다.

UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount) : 이 메서드는 캐릭터가 탄약을 주웠을 때 호출됩니다. 주어진 무기 유형에 대한 탄약 양을 업데이트하고, 필요한 경우 즉시 재장전을 시작합니다.

UCombatComponent::BeginPlay() : 이 메서드는 게임 시작 시 호출되며, 캐릭터의 움직임 속도와 카메라의 기본 시야를 설정합니다.

UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) : 이 메서드는 프레임마다 호출되며, 캐릭터가 로컬로 제어되는 경우, 십자선 아래에 있는 물체를 추적하고 HUD 십자선을 설정하고 시야를 보간합니다.

UCombatComponent::FireButtonPressed(bool bPressed) : 이 메서드는 불을 지휘하는 버튼이 눌렸을 때 호출됩니다. 눌린 경우, Fire 메서드를 호출합니다.

UCombatComponent::Fire(), UCombatComponent::FireProjectileWeapon(), UCombatComponent::FireHitScanWeapon(), UCombatComponent::FireShotgun() : 이들 메서드는 캐릭터가 무기를 발사할 때 사용되며, 무기의 종류에 따라 다른 메서드가 호출됩니다.

UCombatComponent::StartFireTimer(), UCombatComponent::FireTimerFinished() : 이 메서드들은 발사 간의 지연을 관리합니다.
  
void UCombatComponent::Fire(): 이 메서드는 캐릭터가 무기를 쏠 수 있는지 검사한 후, 무기에 따른 적절한 발사 방법을 실행합니다. 발사 후에는 StartFireTimer를 호출하여 발사 대기 시간을 설정합니다.

void UCombatComponent::FireProjectileWeapon(): 이 메서드는 투사체 무기를 발사합니다. 이 메서드는 서버에서 권한이 있는지 확인하고, 해당 무기가 흩뿌리기 방식을 사용하는지 여부에 따라 투사체가 명중할 위치를 조정합니다.

void UCombatComponent::FireHitScanWeapon(): 이 메서드는 히트스캔 무기를 발사합니다. 투사체 무기를 발사하는 메서드와 유사하게 작동하지만, 히트스캔 무기는 즉시 목표에 명중하는 것을 시뮬레이트합니다.

void UCombatComponent::FireShotgun(): 이 메서드는 샷건을 발사합니다. 이 메서드는 샷건의 특성에 따라 여러 탄환을 동시에 발사합니다.

void UCombatComponent::StartFireTimer(): 이 메서드는 무기의 발사 대기 시간을 설정합니다. 설정된 시간이 지나면 FireTimerFinished 메서드가 호출됩니다.

void UCombatComponent::FireTimerFinished(): 이 메서드는 발사 대기 시간이 지난 후 호출됩니다. 이 메서드는 캐릭터가 계속 발사하려고 하는지 확인하고, 캐릭터가 자동 무기를 장착하고 있으면 다시 발사합니다. 또한, 무기가 비었는지 확인하고 필요하면 재장전을 시도합니다.

void UCombatComponent::ServerFire_Implementation(), bool UCombatComponent::ServerFire_Validate(): 이 두 메서드는 무기 발사를 위한 서버 사이드 유효성 검사 및 구현을 처리합니다.

void UCombatComponent::MulticastFire_Implementation(): 이 메서드는 무기 발사를 클라이언트에게 다중 송신합니다.

void UCombatComponent::ServerShotgunFire_Implementation(), bool UCombatComponent::ServerShotgunFire_Validate(): 이 두 메서드는 샷건 발사를 위한 서버 사이드 유효성 검사 및 구현을 처리합니다.

void UCombatComponent::MulticastShotgunFire_Implementation(): 이 메서드는 샷건 발사를 클라이언트에게 다중 송신합니다.

void UCombatComponent::LocalFire(): 이 메서드는 클라이언트에서 무기 발사를 처리합니다. 이 메서드는 캐릭터의 현재 상태와 무기의 상태에 따라 적절한 발사 방법을 실행하거나, 발사를 실패하게 됩니다.

void UCombatComponent::Reload(): 이 메서드는 캐릭터가 무기를 재장전하도록 요청합니다. 재장전 요청이 유효하면 StartReloadTimer를 호출하여 재장전 대기 시간을 설정합니다.

void UCombatComponent::StartReloadTimer(): 이 메서드는 재장전 대기 시간을 설정합니다. 설정된 시간이 지나면 ReloadTimerFinished 메서드가 호출됩니다.

void UCombatComponent::ReloadTimerFinished(): 이 메서드는 재장전 대기 시간이 지난 후 호출됩니다. 재장전이 완료되면 무기의 탄약 상태를 업데이트합니다.

void UCombatComponent::ServerReload_Implementation(), bool UCombatComponent::ServerReload_Validate(): 이 두 메서드는 재장전을 위한 서버 사이드 유효성 검사 및 구현을 처리합니다.

void UCombatComponent::MulticastReload_Implementation(): 이 메서드는 재장전을 클라이언트에게 다중 송신합니다.

void UCombatComponent::LocalReload(): 이 메서드는 클라이언트에서 재장전을 처리합니다. 이 메서드는 캐릭터의 현재 상태와 무기의 탄약 상태에 따라 적절한 재장전 방법을 실행하거나, 재장전을 실패하게 됩니다.

void UCombatComponent::CancelReload(): 이 메서드는 캐릭터가 재장전을 중단하도록 요청합니다. 재장전 중단 요청이 유효하면 ReloadTimerFinished 메서드의 호출을 취소합니다.

void UCombatComponent::ServerCancelReload_Implementation(), bool UCombatComponent::ServerCancelReload_Validate(): 이 두 메서드는 재장전 취소를 위한 서버 사이드 유효성 검사 및 구현을 처리합니다.

void UCombatComponent::MulticastCancelReload_Implementation(): 이 메서드는 재장전 취소를 클라이언트에게 다중 송신합니다.

void UCombatComponent::LocalCancelReload(): 이 메서드는 클라이언트에서 재장전 취소를 처리합니다. 이 메서드는 캐릭터의 현재 상태에 따라 적절한 재장전 취소 방법을 실행합니다.
  
---
  
# LagCompensationComponent.h
  
