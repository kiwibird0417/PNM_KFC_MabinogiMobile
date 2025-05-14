#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerSkillData.h"
#include "PlayerClassSkillInterface.generated.h"

// 1. UInterface 클래스
UINTERFACE(MinimalAPI)
class UPlayerClassSkill : public UInterface
{
	GENERATED_BODY()
};

// 2. 실제 인터페이스 클래스
class PNM_KFC_API IPlayerClassSkill
{
	GENERATED_BODY() 

public:
	virtual void UseAttack(class APlayerCharacter* Character)= 0;

	virtual void SetSkillDataList(const TArray<FPlayerSkillData>& Skills) = 0;

	virtual void UseSkill(class APlayerCharacter* Character , int32 SkillIndex) = 0;
};
