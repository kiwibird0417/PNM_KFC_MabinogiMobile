#include "ODH_EnemySpawner/EnemySpawner.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//정해진 수 만큼 에너미를 소환
	for ( int32 i = 0; i < MaxEnemyCount; i++ )
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATestEnemy* enemy = GetWorld()->SpawnActor<ATestEnemy>(SetEnemy, params);

		//소환된 에너미를 오브젝트 풀에 저장
		//EnemyList.Add(enemy);
		//다른 배열이 생기면서 이건 안해도 되게되었음
		
		//생성된 에너미에게 자신이 리스트 몇번인지 보내줌
		/*enemy->FSM->ListNum = i;*/
		//생성된 에너미에게 자신의 스포너가 누구인지 알려줌
		/*enemy->FSM->Spawner = this;*/

		//해당 에너미들을 게임에서 안 보이고 충돌 처리가 안되도록 만듦
		enemy->SetActorHiddenInGame(true);
		enemy->SetActorEnableCollision(false);
		//대기(사망) 리스트에 넣음
		ObjectPool.Add(enemy);
	}

	SpawnEnemy();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//리스트 안에 있는 에너미가 공격받을 경우 불려오는 함수
void AEnemySpawner::EnemyOnDamage(int32 listNum)
{	
	//자신의 에너미 리스트만큼 반복해서
	for ( int32 i = 0; i < EnemyList.Num(); i++ )
	{
		if(i==listNum) continue;
		
		//모든 에너미들을 추격 상태로 변환한다.
		EnemyList[i]->FSM->mState = EDungeonEnemyState::CHASE;
	}
}

void AEnemySpawner::CallEnemyDie(ATestEnemy* enemy)
{
	enemy->SetActorHiddenInGame(true);
	enemy->SetActorEnableCollision(false);
	EnemyList.Remove(enemy);
	ObjectPool.Add(enemy);
}

void AEnemySpawner::CheckEnemiesValid()
{
	
}

void AEnemySpawner::SpawnEnemy()
{
	for ( int32 i = 0; i < MaxEnemyCount; i++ )
	{
		//오브젝트 풀의 맨앞에 것의 정보를 가져오고 삭제함
		ATestEnemy* enemy = ObjectPool[0];
		ObjectPool.RemoveAt(0);

		enemy->SetActorHiddenInGame(false);
		enemy->SetActorEnableCollision(true);
		//enemy->SetActorLocation(enemy->FSM->Spawner->GetActorLocation());
		EnemyList.Add(enemy);
	}
}

