#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerClassSkillInterface.h"
#include "PlayerSkillData.h"
#include "UMageSkill.generated.h"

UCLASS()
class PNM_KFC_API UMageSkill : public UObject , public IPlayerClassSkill
{
	GENERATED_BODY()

public:
	UMageSkill();
	virtual ~UMageSkill();

public:
	virtual void UseAttack(APlayerCharacter* Character) override;

	virtual void SetSkillDataList(const TArray<FPlayerSkillData>& Skills) override;
	
	virtual void UseSkill(APlayerCharacter* Character, int32 SkillIndex) override;

	//--------------------------------------------------------------------------
	//UPROPERTY(EditDefaultsOnly, Category = "Skill")
	//TSubclassOf<class ANormalProjectile> ProjectileClass;
	UClass* LoadedProjectileClass = nullptr;


	//--------------------------------------------------------------------------
	TArray<FPlayerSkillData> MageSkills;
	
	//--------------------------------------------------------------------------
	// Electro Skill용
	UClass* ElectroProjectileClass;

	// 전기 스킬
	void ElectroSkill(APlayerCharacter* Character);

	// 불 스킬
	void FireSkill();

};
