// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_EnemyAnim/EnemyAnim.h"
#include "ODH_Enemy/TestEnemy.h"

void UEnemyAnim::AnimNotify_OnAttack()
{
	if( !me )
	{
		me = Cast<ATestEnemy>(TryGetPawnOwner());
	}

	if ( me )
	{	
		me->FSM->StartAttackPlayer();
	}
}

void UEnemyAnim::AnimNotify_EndAttack()
{
	if ( !me )
	{
		me = Cast<ATestEnemy>(TryGetPawnOwner());
	}
	
	if ( me )
	{
		me->FSM->EndAttackPlayer();
	}
}

void UEnemyAnim::AnimNotify_DieEnemy()
{
	if ( !me )
	{
		me = Cast<ATestEnemy>(TryGetPawnOwner());
	}

	if ( me )
	{
		me->FSM->DieEnemy();
	}
}