// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_EnemySpawner/DungeonEnemySpawner.h"
#include "Components/BoxComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"

// Sets default values
ADungeonEnemySpawner::ADungeonEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(L"Collision");
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(32,280,190));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADungeonEnemySpawner::OnPlayer);

	SpawnPos1 = CreateDefaultSubobject<USceneComponent>(L"SpanwPos1");
	SpawnPos1->SetupAttachment(RootComponent);

	SpawnPos2 = CreateDefaultSubobject<USceneComponent>(L"SpanwPos2");
	SpawnPos2->SetupAttachment(RootComponent);

	SpawnPos3 = CreateDefaultSubobject<USceneComponent>(L"SpanwPos3");
	SpawnPos3->SetupAttachment(RootComponent);

	SpawnPos4 = CreateDefaultSubobject<USceneComponent>(L"SpanwPos4");
	SpawnPos4->SetupAttachment(RootComponent);

	SpawnPos5 = CreateDefaultSubobject<USceneComponent>(L"SpanwPos5");
	SpawnPos5->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADungeonEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPosList.Add(SpawnPos1);
	SpawnPosList.Add(SpawnPos2);
	SpawnPosList.Add(SpawnPos3);
	SpawnPosList.Add(SpawnPos4);
	SpawnPosList.Add(SpawnPos5);
}

// Called every frame
void ADungeonEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//웨이브가 막바지이고 모든 에너미가 죽었을 경우
	if ( NextDoor && IndexPoint >= WaveCount && SpawnEnemys.IsEmpty() )
	{
		NextDoor->OpenDoor();
	}

	//자리 포인터 값이 총 웨이브보다 높지 않을경우에만 실행
	if(IndexPoint <  WaveCount)
	{ 
		//만약 웨이브 적들이 전부 죽으면
		if ( !BoxComp->IsCollisionEnabled() && SpawnEnemys.IsEmpty() )
		{
			CurSpawnTime+=DeltaTime;
			if ( CurSpawnTime >= SpawnDelayTime )
			{
				//두번째 웨이브 적들을 소환
				//1웨이브면 1웨이브의, 2웨이브면 2웨이브의 적들을 소환
				WaveSpawn(EnemyType[IndexPoint] , EnemyCounts   [IndexPoint]);
				IndexPoint++; //같은 웨이브가 소환되지 않도록 미리 1증가 시킴
				CurSpawnTime = 0.0f;
			}
		}
	}
}

void ADungeonEnemySpawner::RemoveEnemyList()
{
	//배열에서 빼줌
	SpawnEnemys.RemoveAt(0);
}

void ADungeonEnemySpawner::WaveSpawn(int32 enemyType , int32 enemyCount)
{		
	if ( HasAuthority() )
	{
		ServerRPC_WaveSpawn(enemyType,enemyCount);
	}
}

void ADungeonEnemySpawner::ServerRPC_WaveSpawn_Implementation(int32 type , int32 count)
{
	switch ( type )
	{
	default:
		break;
	case 0:	//바게스트 소환
		for ( int32 i = 0; i < count; i++ )
		{
			ATestEnemy* enemy = GetWorld()->SpawnActor<ATestEnemy>(BargestFactory , SpawnPosList[i]->GetComponentTransform());
			//해당 에너미에게 스포너 자신의 정보를 줌
			enemy->FSM->Spawner = this;
			enemy->FSM->LoadStat(TEXT("Bargest"));
			SpawnEnemys.Add(enemy);
		}
		break;
	case 1:	//미믹 소환
		for ( int32 i = 0; i < count; i++ )
		{
			ATestEnemy* enemy = GetWorld()->SpawnActor<ATestEnemy>(MimicFactory , SpawnPosList[i]->GetComponentTransform());
			//해당 에너미에게 스포너 자신의 정보를 줌
			enemy->FSM->Spawner = this;
			enemy->FSM->LoadStat(TEXT("Mimic"));
			SpawnEnemys.Add(enemy);
		}
		break;
	case 2:	//벌레 소환
		for ( int32 i = 0; i < count; i++ )
		{
			ATestEnemy* enemy = GetWorld()->SpawnActor<ATestEnemy>(BugFactory , SpawnPosList[i]->GetComponentTransform());
			//해당 에너미에게 스포너 자신의 정보를 줌
			enemy->FSM->Spawner = this;
			enemy->FSM->LoadStat(TEXT("Bug"));
			SpawnEnemys.Add(enemy);
		}
		break;
	}
}

void ADungeonEnemySpawner::OnPlayer(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	if ( HasAuthority() )
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

		if ( player )
		{
			//첫번째 웨이브, 0의 위치한 배열들을 불러옴
			WaveSpawn(EnemyType[IndexPoint] , EnemyCounts[IndexPoint]);
			IndexPoint++; //같은 웨이브가 소환되지 않도록 미리 1증가시킴

			//다시 충돌 감지가 일어나지 않게 꺼줌
			BoxComp->SetCollisionEnabled	(ECollisionEnabled::NoCollision);
		}
	}
}

