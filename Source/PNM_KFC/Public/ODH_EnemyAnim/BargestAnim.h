// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"
#include "BargestAnim.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UBargestAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	EDungeonEnemyState AnimState = EDungeonEnemyState::IDLE;
};
