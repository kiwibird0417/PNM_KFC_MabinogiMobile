#include "AHS_Player/AHS_PlayerAttack/MageAttack/ElectroProjectile.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "AHS_Player/PlayerCharacter.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Boss/BossEnemy.h"
#include "AHS_Interactables/InteractableItem.h"
#include "Components/SceneComponent.h"

AElectroProjectile::AElectroProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent); 

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);      


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->ProjectileGravityScale = 0.f;


	CollisionComp->OnComponentBeginOverlap.AddDynamic(this , &AElectroProjectile::OnOverlap);
}




void AElectroProjectile::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if ( PC )
	{
		Player = Cast<APlayerCharacter>(PC->GetPawn());
	}

	if ( Player && Player->CurrentTarget )
	{
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = Player->CurrentTarget->GetActorLocation();
		FVector Direction = ( TargetLocation - StartLocation ).GetSafeNormal();

		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	}
	else
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
	}
}

void AElectroProjectile::Tick(float DeltaTime)
{

}

void AElectroProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
	UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	if ( !OtherActor || OtherActor == this || !OtherComp )
	{
		return;
	}

	FVector HitLocation = SweepResult.ImpactPoint;

	// ImpactPoint가 (0,0,0)이면 fallback
	if ( HitLocation.IsNearlyZero() )
	{
		HitLocation = OtherActor->GetActorLocation();
	}

	bool bHitEnemy = false;

	ATestEnemy* TestEnemy = Cast<ATestEnemy>(OtherActor);
	ABossEnemy* BossEnemy = Cast<ABossEnemy>(OtherActor);
	AInteractableItem* Interactable = Cast<AInteractableItem>(OtherActor);

	if ( TestEnemy || BossEnemy || Interactable )
	{
		bHitEnemy = true;

		// 감전 이펙트가 있다면 여기에서 재생 가능
		// 예: UGameplayStatics::SpawnEmitterAtLocation(), 사운드, 데미지 등

		DrawDebugSphere(GetWorld() , HitLocation , 30.f , 12 , FColor::Green , false , 2.f);

		float ChainDistance = 500.f;
		int MaxChains = 5;
		ChainLightning_SortByDistance(OtherActor , ChainDistance , MaxChains);
	}
	else
	{
		DrawDebugSphere(GetWorld() , HitLocation , 30.f , 12 , FColor::Red , false , 2.f);
	}

	Destroy();
}

void AElectroProjectile::ChainLightning_BFS(AActor* StartEnemy , float MaxDistance , int MaxChains)
{
	TQueue<AActor*> Queue;
	TSet<AActor*> Visited;

	Queue.Enqueue(StartEnemy);
	Visited.Add(StartEnemy);

	int ChainCount = 0;

	while ( !Queue.IsEmpty() && ChainCount < MaxChains )
	{
		AActor* Current;
		Queue.Dequeue(Current);

		TArray<FOverlapResult> Overlaps;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxDistance);

		GetWorld()->OverlapMultiByChannel(
			Overlaps ,
			Current->GetActorLocation() ,
			FQuat::Identity ,
			ECC_Pawn ,
			Sphere
		);

		for ( auto& Result : Overlaps )
		{
			AActor* NearbyEnemy = Result.GetActor();
			if ( NearbyEnemy && !Visited.Contains(NearbyEnemy) && NearbyEnemy != Current )
			{
				DrawDebugLine(GetWorld() , Current->GetActorLocation() , NearbyEnemy->GetActorLocation() , FColor::Blue , false , 1.5f , 0 , 3.f);

				Queue.Enqueue(NearbyEnemy);
				Visited.Add(NearbyEnemy);
				ChainCount++;

				if ( ChainCount >= MaxChains )
					break;
			}
		}
	}
}

void AElectroProjectile::ChainLightning_SortByDistance(AActor* StartEnemy , float MaxDistance , int MaxChains)
{
	TArray<AActor*> FoundEnemies;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxDistance);

	GetWorld()->OverlapMultiByChannel(
		Overlaps ,
		StartEnemy->GetActorLocation() ,
		FQuat::Identity ,
		ECC_Pawn , // 필요시 Custom Channel로 변경
		Sphere
	);

	for ( auto& Result : Overlaps )
	{
		AActor* NearbyEnemy = Result.GetActor();
		if ( NearbyEnemy && NearbyEnemy != StartEnemy )
		{
			// 적 타입인지 확인
			if ( Cast<ATestEnemy>(NearbyEnemy) || Cast<ABossEnemy>(NearbyEnemy) || Cast<AInteractableItem>(NearbyEnemy))
			{
				FoundEnemies.Add(NearbyEnemy);
			}
		}
	}

	UE_LOG(LogTemp , Warning , TEXT("Found %d valid enemies around %s") , FoundEnemies.Num() , *StartEnemy->GetName());


	// 거리순 정렬
	FoundEnemies.Sort([&](AActor& A , AActor& B) {
		return FVector::Dist(StartEnemy->GetActorLocation() , A.GetActorLocation()) <
			FVector::Dist(StartEnemy->GetActorLocation() , B.GetActorLocation());
	});

	int ChainCount = 0;
	AActor* From = StartEnemy;

	for ( AActor* Target : FoundEnemies )
	{
		if ( ChainCount >= MaxChains )
			break;

		// 디버그 라인 그리기
		DrawDebugLine(GetWorld() ,
					  From->GetActorLocation() ,
					  Target->GetActorLocation() ,
					  FColor::Yellow ,
					  false ,
					  1.5f ,
					  0 ,
					  3.f);

		From = Target;
		ChainCount++;
	}
}
