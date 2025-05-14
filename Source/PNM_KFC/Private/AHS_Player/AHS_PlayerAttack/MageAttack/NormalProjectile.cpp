// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Player/AHS_PlayerAttack/MageAttack/NormalProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Boss/BossEnemy.h"
#include "AHS_PlayerComponent/AC_PlayerDamageManagerComponent.h"

ANormalProjectile::ANormalProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// 외형 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	// 충돌 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	Collision->OnComponentBeginOverlap.AddDynamic(this , &ANormalProjectile::OnOverlap);
	MeshComp->SetupAttachment(RootComponent);

}

void ANormalProjectile::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if ( PC )
	{
		Player = Cast<APlayerCharacter>(PC->GetPawn());
	}
}

void ANormalProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( Player && Player->CurrentTarget )
	{
		FVector StartLocation = Player->GetActorLocation();
		FVector TargetLocation = Player->CurrentTarget->GetActorLocation();

		InitProjectile(StartLocation , TargetLocation);
		// 나머지 처리
	}
	else
	{
		
	}

}

void ANormalProjectile::InitProjectile(FVector StartLocation , FVector TargetLocation)
{
	// 에너미를 향해 일직선으로 날아가기

	// 충돌을 했을 때, Destroy

	FVector Direction = ( TargetLocation - StartLocation ).GetSafeNormal();
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}

void ANormalProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
								  UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
								  const FHitResult& SweepResult)
{

	// 부딪힌 위치 확인용 디버그 구체
	FVector HitLocation = SweepResult.ImpactPoint;
	bool bHitEnemy = false;

	
	ATestEnemy* TestEnemy = Cast<ATestEnemy>(OtherActor);
	if ( TestEnemy != nullptr ) {
		// 일반 에너미 데미지 로직
		bHitEnemy = true;

		// 일반 적에게 데미지 로직 적용		
		Player->PlayerDamageManageComp->ApplyDamageToEnemy(20);
	}

	ABossEnemy* BossEnemy = Cast<ABossEnemy>(OtherActor);
	if ( BossEnemy != nullptr ) {
		// 보스 에너미 데미지 로직
		bHitEnemy = true;

		// 보스 적에게 데미지 로직 적용		
		Player->PlayerDamageManageComp->ApplyDamageToEnemy(30);
	}



	//-------------------------------------------------------
	// 공통 실행
	

	// 디버그 구체 (성공: 초록색, 실패: 빨간색)
	FColor SphereColor = bHitEnemy ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld() , HitLocation , 30.f , 12 , SphereColor , false , 2.f);


	// 에너미라면 VFX 표시
	if ( bHitEnemy ) {

	}
	else {
		// 그 외라면(플레이어, 벽, 바닥 충돌시) 아무것도 없음
	}

	Destroy();
}