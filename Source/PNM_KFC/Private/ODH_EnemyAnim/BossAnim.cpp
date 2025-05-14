// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_EnemyAnim/BossAnim.h"
#include "ODH_Boss/BossEnemy.h"
#include "ODH_Boss/BossFSM.h"

void UBossAnim::AnimNotify_AttackStart()
{
	if ( !me )
	{
		me = Cast<ABossEnemy>(TryGetPawnOwner());
	}

	if ( me )
	{
		me->FSM->StartAttackPlayer();
	}
}

void UBossAnim::AnimNotify_AttackEnd()
{
	if ( me )
	{
		me->FSM->EndAttackPlayer();

		AttackType = 0;
	}
}

void UBossAnim::AnimNotify_SwingAttack()
{
	if ( !me )
	{
		me = Cast<ABossEnemy>(TryGetPawnOwner());
	}

	if ( me )
	{
		me->FSM->StartSwingAttack();
	}
}

void UBossAnim::AnimNotify_SwingAttackEnd()
{
	if ( me )
	{
		me->FSM->EndSwingAttack();
	}
}

void UBossAnim::AnimNotify_GoToPlayer()
{
	if ( !me )
	{
		me = Cast<ABossEnemy>(TryGetPawnOwner());
	}

	if ( me )
	{
		me->FSM->GoToPlayer();
	}
}

void UBossAnim::AnimNotify_StompAttack()
{
	if ( me )
	{
		me->FSM->StartStompAttack();
	}
}

void UBossAnim::AnimNotify_Skill3Attack()
{
	if ( !me )
	{
		me = Cast<ABossEnemy>(TryGetPawnOwner());
	}

	if ( me )
	{
		me->FSM->Skill3Attack();
	}
}

void UBossAnim::AnimNotify_DieEnemy()
{
	if ( me )
	{
		me->FSM->DieEnemy();
	}
}
