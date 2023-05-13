# BuffComponent.h

UBuffComponent::Heal(float HealAmount, float HealingTime): 이 함수는 주어진 시간 동안 캐릭터의 체력을 점진적으로 회복하는 버프를 설정합니다.

UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime): 이 함수는 주어진 시간 동안 캐릭터의 방패를 점진적으로 회복하는 버프를 설정합니다.

UBuffComponent::HealRampUp(float DeltaTime): 이 함수는 "Heal" 버프를 실행하는 메서드입니다. 이는 프레임당 호출되어 캐릭터의 체력을 점진적으로 회복합니다.

UBuffComponent::ShieldRampUp(float DeltaTime): 이 함수는 "ReplenishShield" 버프를 실행하는 메서드입니다. 이는 프레임당 호출되어 캐릭터의 방패를 점진적으로 회복합니다.

UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed): 이 함수는 캐릭터의 원래 이동 속도와 웅크린 상태에서의 이동 속도를 설정합니다.

UBuffComponent::SetInitialJumpVelocity(float Velocity): 이 함수는 캐릭터의 원래 점프 속도를 설정합니다.

UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime): 이 함수는 주어진 시간 동안 캐릭터의 이동 속도와 웅크린 상태에서의 이동 속도를 일시적으로 증가시킵니다.

UBuffComponent::ResetSpeeds(): 이 함수는 "BuffSpeed" 버프가 끝난 후 캐릭터의 이동 속도를 원래대로 되돌립니다.

UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime): 이 함수는 주어진 시간 동안 캐릭터의 점프 속도를 일시적으로 증가시킵니다.

UBuffComponent::ResetJump(): 이 함수는 "BuffJump" 버프가 끝난 후 캐릭터의 점프 속도를 원래대로 되돌립니다.

UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction): 이 함수는 매 프레임마다 호출되며, 이때 HealRampUp 및 ShieldRampUp 함수를 호출하여 체력 및 방패 회복 버프를 적용합니다.

이 클래스는 게임 캐릭터의 능력치를 임시적으로 증가시키는데 사용됩니다.

---

# CombatComponent.h

