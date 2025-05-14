// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"
#include "Components/SphereComponent.h"
#include "ODH_EnemyAnim/EnemyAnim.h"
#include "Components/WidgetComponent.h"
#include "ODH_EnemyUI/EnemyUI.h"
#include "AHS_Player/PlayerCharacter.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

ATestEnemy::ATestEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp=CreateDefaultSubobject<USphereComponent>(L"SphereComp");
	SphereComp->SetupAttachment(GetMesh());
	SphereComp->SetSphereRadius(450);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ATestEnemy::InteractionPlayer);

	UIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPUIComp"));
	UIComp->SetupAttachment(GetMesh());
}

void ATestEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME(ATestEnemy, EnemyHPUI);*/
}

void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();

	FSM = GetComponentByClass<UDungeonEnemyFSM>();

	EnemyHPUI = Cast<UEnemyUI>(UIComp->GetWidget());

	if ( isBargest )
	{
		FSM->LoadStat(TEXT("Bargest"));
		EnemyHPUI->Name = TEXT("바게스트");
		UIComp->SetVisibility(false);
	}

	else if ( isMimic )
	{
		FSM->LoadStat(TEXT("Mimic"));
		EnemyHPUI->Name = TEXT("미믹");
		UIComp->SetVisibility(false);
	}

	else if ( isBug )
	{
		FSM->LoadStat(TEXT("Bug"));
		EnemyHPUI->Name = TEXT("벌레");
		UIComp->SetVisibility(false);
	}

	else if ( isTest )
	{
		FSM->LoadStat(TEXT("Test"));
		EnemyHPUI->Name = TEXT("테스트용");
	}
}

void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSM)
	{
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"FSM is True");
	}
}

void ATestEnemy::InteractionPlayer(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if ( player )
	{
		FSM->Target = player;

		FSM->mState= EDungeonEnemyState::CHASE;
		FSM->Anim->AnimState = FSM->mState;
		UIComp->SetVisibility(true);

		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ATestEnemy::EnemyOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	//공격을 맞을 경우 피격 모션이 나오도록 변경
	FSM->mState = EDungeonEnemyState::DAMAGE;
	FSM->Anim->AnimState = FSM->mState;
	//공격을 맞았을 경우 해당 공격이 어떤 플레이어인지 어떻게 알 수 있을까?
}

// void ATestEnemy::DamageProcess(int32 damage)
// {
// 	float newHP = FSM->CurHP-=damage;
// 	FSM->SetHP(newHP);
// }

