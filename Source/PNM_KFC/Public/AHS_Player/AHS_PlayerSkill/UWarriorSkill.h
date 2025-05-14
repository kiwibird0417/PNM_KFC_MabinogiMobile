#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerClassSkillInterface.h"
#include "PlayerSkillData.h"
#include "UWarriorSkill.generated.h"

UCLASS()
class PNM_KFC_API UWarriorSkill : public UObject, public IPlayerClassSkill
{
	GENERATED_BODY()

public:
	UWarriorSkill();
	virtual ~UWarriorSkill();

public:
	virtual void UseAttack(APlayerCharacter* Character) override;

	virtual void SetSkillDataList(const TArray<FPlayerSkillData>& Skills) override;
	
	virtual void UseSkill(APlayerCharacter* Character, int32 SkillIndex) override;


	//--------------------------------------------------------------------------
	TArray<FPlayerSkillData> WarriorSkills;
	
	// UWarriorSkill.h에 SkillDataList 받아오기 위한 setter 추가
	//void SetSkillDataList(const TArray<FPlayerSkillData>& Skills);

};
