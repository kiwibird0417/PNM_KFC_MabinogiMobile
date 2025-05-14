// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ODH_Boss/BossFSM.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = FSM)
	EBossState AnimState = EBossState::IDLE;

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_SwingAttack();

	UFUNCTION()
	void AnimNotify_SwingAttackEnd();

	UFUNCTION()
	void AnimNotify_GoToPlayer();

	UFUNCTION()
	void AnimNotify_StompAttack();

	UFUNCTION()
	void AnimNotify_Skill3Attack();

	UFUNCTION()
	void AnimNotify_DieEnemy();

	//번호에 따라 실행되는 공격이 달라짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	int32 AttackType = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* DeathMontge;

private:
	class ABossEnemy* me;
};
