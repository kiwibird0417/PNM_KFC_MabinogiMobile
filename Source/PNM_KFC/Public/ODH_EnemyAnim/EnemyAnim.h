// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"
#include "EnemyAnim.generated.h"

UCLASS()
class PNM_KFC_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	EDungeonEnemyState AnimState = EDungeonEnemyState::IDLE;

	UFUNCTION()
	void AnimNotify_OnAttack();

	UFUNCTION()
	void AnimNotify_EndAttack();

	UFUNCTION()
	void AnimNotify_DieEnemy();

private:
	class ATestEnemy* me;
};
