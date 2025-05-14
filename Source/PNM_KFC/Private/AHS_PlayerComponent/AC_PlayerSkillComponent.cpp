// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_PlayerComponent/AC_PlayerSkillComponent.h"
#include "AHS_Player/PlayerCharacter.h"

#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"

// 클래스별 스킬 연결
#include "AHS_Player/AHS_PlayerSkill/UWarriorSkill.h"
#include "AHS_Player/AHS_PlayerSkill/UMageSkill.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerSkillData.h"





// Sets default values for this component's properties
UAC_PlayerSkillComponent::UAC_PlayerSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_PlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// 플레이어 컨트롤러 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	
}


// Called every frame
void UAC_PlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


//---------------------------------------------------------------------------------------
// 일반 공격 사용



//---------------------------------------------------------------------------------------
// 스킬 사용
void UAC_PlayerSkillComponent::UseSkill(int32 Index)
{
	if ( SkillStrategy )
	{
		SkillStrategy->UseSkill(OwnerCharacter , Index);
	}
	else {
		UE_LOG(LogTemp , Warning , TEXT("No SkillStrategy set or OwnerCharacter invalid"));
	}
}

/*
void UAC_PlayerSkillComponent::SetSkillStrategy(EPlayerClassType PlayerClass)
{
	switch ( PlayerClass )
	{
	case EPlayerClassType::Warrior:
		SkillObject = NewObject<UWarriorSkill>(this); 
		break;

	case EPlayerClassType::Mage:
		SkillObject = NewObject<UMageSkill>(this); 
		break;

		// Mage, Archer 등 다른 직업도 여기에
	default:
		SkillObject = nullptr;
		break;
	}

	SkillStrategy = Cast<IPlayerClassSkill>(SkillObject);
}
*/

void UAC_PlayerSkillComponent::SetSkillStrategy(EPlayerClassType PlayerClass)
{
	switch ( PlayerClass )
	{
	case EPlayerClassType::Warrior:
		SkillObject = NewObject<UWarriorSkill>(this);
		break;

	case EPlayerClassType::Mage:
		SkillObject = NewObject<UMageSkill>(this);
		break;

	default:
		SkillObject = nullptr;
		break;
	}

	SkillStrategy = Cast<IPlayerClassSkill>(SkillObject);

	// SkillDataList도 전달
	if ( SkillStrategy )
	{
		if ( auto* WarriorSkill = Cast<UWarriorSkill>(SkillStrategy) )
		{
			WarriorSkill->SetSkillDataList(SkillDataList);
		}
		else if ( auto* MageSkill = Cast<UMageSkill>(SkillStrategy) )
		{
			MageSkill->SetSkillDataList(SkillDataList);
		}
	}
}


//-----------------------------------------------------------------------------
void UAC_PlayerSkillComponent::SetSkillDataList(const TArray<FPlayerSkillData>& Skills)
{
	SkillDataList = Skills;

}

//-----------------------------------------------------------------------------
// 일반 공격
void UAC_PlayerSkillComponent::UseAttack()
{
	if ( SkillStrategy )
	{
		SkillStrategy->UseAttack(OwnerCharacter);
	}
}

//-----------------------------------------------------------------------------
// 마법사 스킬을 위한... 함수




