// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_PlayerClassComponent.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerClassSkillInterface.h"
#include "AC_PlayerSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UAC_PlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_PlayerSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//--------------------------------------------------------------
public:
	// 플레이어 캐릭터 참조
	class APlayerCharacter* OwnerCharacter;

//--------------------------------------------------------------
	UFUNCTION()
	void UseSkill(int32 SkillIndex);


//--------------------------------------------------------------
public:
	UPROPERTY()
	UObject* SkillObject; // 인터페이스는 UPROPERTY로 저장 불가

	IPlayerClassSkill* SkillStrategy;

	void SetSkillStrategy(EPlayerClassType PlayerClass);


//--------------------------------------------------------------
	TArray<FPlayerSkillData> SkillDataList;

	void SetSkillDataList(const TArray<FPlayerSkillData>& Skills);

//--------------------------------------------------------------
// 일반 공격
public:
	void UseAttack();

		
};
