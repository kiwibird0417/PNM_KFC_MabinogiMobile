// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_PlayerComponent/AC_PlayerDamageManagerComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Boss/BossEnemy.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"
#include "ODH_Boss/BossFSM.h"

// Sets default values for this component's properties
UAC_PlayerDamageManagerComponent::UAC_PlayerDamageManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UAC_PlayerDamageManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 플레이어 컨트롤러 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
}


// Called every frame
void UAC_PlayerDamageManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 플레이어가, 에너미에게 공격을 하는 함수( 데미지 값만 주고 받는 형식임)
void UAC_PlayerDamageManagerComponent::ApplyDamageToEnemy(int damage)
{
	// 실제로 가하는 데미지
	//damage = 20;
	int32 skilldamage = 50;

	if ( OwnerCharacter->bUsingSkill ) {
		damage = 50;
	}

	// 에너미 HP에 데미지 값 반영
	ATestEnemy* enemy = Cast<ATestEnemy>(OwnerCharacter->CurrentTarget);
	ABossEnemy* boss = Cast<ABossEnemy>(OwnerCharacter->CurrentTarget);
	
	if ( enemy != nullptr ) {	
		int newEnemyHP = enemy->FSM->GetHP() - damage;
		enemy->FSM->SetHP(newEnemyHP);
	}
	
	if ( boss != nullptr ) {
		int newEnemyHP = boss->FSM->GetHP() - damage;
		boss->FSM->SetHP(newEnemyHP);
	}


	//UE_LOG(LogTemp, Warning, TEXT("플레이어는 적에게 %d만큼의 데미지를 가했습니다!"), damage);
}

//=========================================================================================
// 플레이어가, 에너미에게 공격을 하는 함수( 데미지 값만 주고 받는 형식임)
void UAC_PlayerDamageManagerComponent::ApplyDamageToPlayer(int damage)
{
	// 실제로 가하는 데미지
	damage = 1;

	// 플레이어 HP에 데미지 값 반영
	int newPlayerHP = OwnerCharacter->GetHP() - damage;
	OwnerCharacter->SetHP(newPlayerHP);

	UE_LOG(LogTemp , Warning , TEXT("적은 플레이어에게 %d만큼의 데미지를 가했습니다!") , damage);
}

