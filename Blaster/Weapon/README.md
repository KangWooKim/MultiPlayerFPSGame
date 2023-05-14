# Casing.h

### ACasing()

생성자에서 케이싱 메시 및 충돌, 물리 속성을 설정


### BeginPlay()

초기 실행 시, 메시 히트 이벤트에 OnHit 메서드 바인딩 및 탄피 추출 추진력 적용


### OnHit(UPrimitiveComponent*, AActor*, UPrimitiveComponent*, FVector, const FHitResult&)

케이싱이 다른 액터와 충돌할 때, 탄피 소리 재생 및 케이싱 파괴

---

# Flag.h

### AFlag()

생성자에서 메시 및 충돌 속성 설정


### Dropped()

깃발을 떨어뜨릴 때 실행되는 메서드로, 무기(깃발) 상태 설정 및 소유자 초기화


### ResetFlag()

깃발을 초기 상태로 리셋하는 메서드로, 깃발을 소유한 캐릭터 속성 변경 및 깃발 상태와 위치 초기화


### OnEquipped()

깃발이 장착되었을 때 실행되는 메서드로, HUD 위젯 숨기고 충돌 및 물리상태 설정


### OnDropped()

깃발이 떨어진 상태일 때 실행되는 메서드로, 충돌, 물리상태, 렌더링 업데이트 처리


### BeginPlay()

초기 실행 시, 초기 깃발 위치를 저장

---

# HitScanWeapon.h

### Fire(const FVector& HitTarget)

무기 발사를 처리하는 메서드로, 무기 가지고 있는 액터와 충돌 검사 후 데미지 적용, 파티클 및 소리 재생 처리


### WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)

발사에 대한 충돌 검사하여 레이를 따라 타겟의 충돌을 확인하며, 빔 파티클 생성 및 충돌 결과 반환

---

# ProJectile.h

### AProjectile()

생성자에서 충돌 상자 및 물리 속성 설정


### BeginPlay()

초기 실행 시, 트레이서 생성 및 'OnHit' 이벤트 바인딩


### OnHit()

프로젝타일이 목표물에 충돌했을 때 실행되는 메서드로, 프로젝타일 파괴 처리


### SpawnTrailSystem()

특정 효과 시스템을 통해 프로젝타일 꼬리 생성


### ExplodeDamage()

프로젝타일 이펙트 반경 내 액터에 대한 데미지와 피해의 범위 적용


### Tick(float DeltaTime)

프로젝타일의 위치 및 회전 갱신 처리


### StartDestroyTimer()

프로젝타일이 파괴될 때까지의 시간을 관리하는 타이머 시작


### DestroyTimerFinished()

지정된 시간 후 프로젝타일 파괴 처리


### Destroyed()

파괴된 프로젝타일에 대한 충돌 이펙트 및 사운드 재생 처리

---

# ProjectileBullet.h

### AProjectileBullet::AProjectileBullet() : 
생성자 메서드로, 프로젝타일 발사체의 기본 속성을 초기화하는 데 사용됩니다. 여기서 발사체 움직임 컴포넌트가 생성되고 필요한 속성 값이 설정됩니다.


### #if WITH_EDITOR void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& Event) #endif: 
에디터에서 속성 변경 이벤트 처리 메서드로, InitialSpeed 속성 값이 변경될 때 발사체 움직임 컴포넌트의 속도도 동기화됩니다.



### void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) : 
발사체가 목표물에 충돌한 경우 호출되는 메서드로, 소유한 플레이어 컨트롤러와 충돌 대상을 확인하여 데미지를 입힙니다. 서버 사이드 리와인드를 사용하면, 서버에 요청사항을 전달합니다.

---

# ProjectileGrenade.h

### AProjectileGrenade::AProjectileGrenade() : 
생성자 메서드로, 수류탄 발사체의 기본 속성을 초기화하는 데 사용됩니다. 발사체 메시와 발사체 움직임 컴포넌트를 생성하며, 움직임 컴포넌트의 속성을 설정한다.


### void AProjectileGrenade::BeginPlay() : 
발사체가 게임 내에서 생성될 때 호출되는 메서드로, 발사체의 트레일 시스템을 생성하고 파괴 타이머를 시작합니다. 발사체 움직임 컴포넌트의 OnProjectileBounce 이벤트에 OnBounce 메서드를 바인딩합니다.


### void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity) : 
발사체가 튕겼을 때 호출되는 메서드로, BounceSound가 설정되어 있으면 튕김 소리를 재생합니다.


### void AProjectileGrenade::Destroyed() : 
발사체가 파괴될 때 호출되는 메서드로, 발사체가 폭발하면 데미지를 처리한 후 상위 클래스의 Destroyed() 메서드를 호출합니다.

---

# ProjectileRocket.h

### AProjectileRocket::AProjectileRocket() : 
생성자 메서드로, 로켓 발사체의 기본 속성을 초기화하는 데 사용됩니다. 발사체 메시와 RocketMovementComponent를 생성하며, 움직임 컴포넌트의 속성을 설정합니다.


### void AProjectileRocket::BeginPlay() : 
발사체가 게임 내에서 생성될 때 호출되는 메서드로, 충돌 이벤트를 처리하고 트레일 시스템을 생성합니다. 루프 사운드가 설정되어 있다면, 이를 재생합니다.


### void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) : 
발사체가 목표물에 충돌한 경우 호출되는 메서드로, 데미지를 처리하고 필요한 시각 및 음향 효과를 처리한 후 발사체를 파괴할 데미지 타이머를 시작합니다.


### void AProjectileRocket::Destroyed() : 
발사체가 파괴될 때 호출되는 메서드입니다. 추가적인 작업을 할 수 있으나, 현재는 빈 메서드로 냅둡니다.

---

# ProjectileWeapon.h

### AProjectileWeapon::Fire(const FVector& HitTarget) : 
무기가 발사되는 메서드로, 발사체를 생성하고 적절한 설정을 적용합니다.

APawn* InstigatorPawn = Cast<APawn>(GetOwner()); 무기 소유자(플레이어)를 가져옵니다.

머즐 플래시 소켓의 위치와 회전각을 구하는 데 사용합니다.

프로젝타일의 대상 위치와 회전을 계산합니다.

서버와 클라이언트에서 발사체를 생성하고 적절한 서버 사이드 리와인드(SSR) 및 데미지 설정을 적용합니다.


이 메서드는 서버와 클라이언트 모두에서 효율적으로 작동하도록 구성되어 있어, 실행 환경에 따른 다양한 발사체 설정이 적용됩니다.
  
---
  
# RocketMovementComponent.h
  
### URocketMovementComponent::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) : 
로켓이 충돌한 경우 처리하는 메서드입니다. 상위 클래스의 HandleBlockingHit을 호출한 후 AdvanceNextSubstep으로 결과를 반환하여 다음 단계로 진행합니다.


### URocketMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) : 
로켓이 충격을 받는 경우 처리하는 메서드입니다. 로켓이 중지되지 않도록 이 메서드는 비어있으며, 로켓은 CollisionBox가 충돌 감지할 때만 폭발합니다.
