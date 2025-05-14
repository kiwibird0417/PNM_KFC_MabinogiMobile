// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Player/PlayerAnimInstance.h"
#include "AHS_Player/PlayerCharacter.h"
//#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Boss/BossEnemy.h"
#include "AHS_PlayerComponent/AC_PlayerSkillComponent.h"
#include "AHS_Player/AHS_PlayerSkill/UMageSkill.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 부모 플레이어 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

//==============================================================================

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

//==============================================================================

// 일반 공격 Anim Montage를 재생한다.
void UPlayerAnimInstance::Play_BasicAttack_Animation()
{
	// 전사 클래스 && Anim Montage가 있다면!
	if ( ( PlayerClass == EPlayerClassType::Warrior  ) && WarriorBasicAttackMontage ) {
		Montage_Play(WarriorBasicAttackMontage);
	}
	else if ( (PlayerClass == EPlayerClassType::Mage ) && MageBasicAttackMontage ) {
		Montage_Play(MageBasicAttackMontage);
	}
}


// 전사 일반 공격
void UPlayerAnimInstance::AnimNotify_OnWarriorBasicAttackFinish()
{
	OwnerCharacter->PlayerSkillComp->UseAttack();
}

// 마법사 일반 공격
void UPlayerAnimInstance::AnimNotify_OnMageBasicAttackFinish()
{
	OwnerCharacter->PlayerSkillComp->UseAttack();
}


//==============================================================================
// Seqence 기반 Notify(미사용)
/*
// 전사 기본 공격 관련 함수 Notify
void UPlayerAnimInstance::AnimNotify_Warrior_BasicAttack()
{
	UE_LOG(LogTemp , Warning , TEXT("전사 공격 처리!"));
}

void UPlayerAnimInstance::AnimNotify_Warrior_BasicAttack_End()
{
	bUseAttack = false;
}

//==============================================================================
// 마법사 기본 공격 관련 함수 Notify
void UPlayerAnimInstance::AnimNotify_Mage_BasicAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("마법사 공격 처리!"));
}


void UPlayerAnimInstance::AnimNotify_Mage_BasicAttack_End()
{
	bUseAttack = false;
}
*/

//==============================================================================
// 전사 스킬 애니메이션 재생
void UPlayerAnimInstance::PlayWarriorSkillAttack(int skillnum)
{
	// 전사 클래스가 아니라면, return
	if ( PlayerClass != EPlayerClassType::Warrior ) {
		return;
	}

	switch ( skillnum ) {
		case 0 : { Montage_Play(WarriorSkill01); } break;
		case 1 : { Montage_Play(WarriorSkill02); } break;
		default: { UE_LOG(LogTemp, Warning, TEXT("No Anim Montage Playing")); }; break;
	}
}

// 연속 베기 1타
void UPlayerAnimInstance::AnimNotify_AN_WarriorSkill01_01()
{
	OwnerCharacter->PlayerSkillComp->UseAttack();
	UE_LOG(LogTemp , Warning , TEXT("연속베기 1타"));
}

// 연속 베기 2타
void UPlayerAnimInstance::AnimNotify_AN_WarriorSkill01_02()
{
	OwnerCharacter->PlayerSkillComp->UseAttack();
	UE_LOG(LogTemp , Warning , TEXT("연속베기 2타"));
}

// 연속 베기 3타
void UPlayerAnimInstance::AnimNotify_AN_WarriorSkill01_03()
{
	OwnerCharacter->PlayerSkillComp->UseAttack();
	UE_LOG(LogTemp , Warning , TEXT("연속베기 3타"));
}

void UPlayerAnimInstance::AnimNotify_AN_WarriorSkill01_End()
{
	// 스킬이 끝나면 스킬 끝내기
	OwnerCharacter->bUsingSkill = false;
	OwnerCharacter->bSkillEnded = true;
}

void UPlayerAnimInstance::AnimNotify_AN_WarriorSkill02_End()
{
	// 스킬이 끝나면 스킬 끝내기
	OwnerCharacter->bUsingSkill = false;
	OwnerCharacter->bSkillEnded = true;
}

//==============================================================================
// 마법사 스킬 애니메이션 재생
void UPlayerAnimInstance::PlayMageSkillAttack(int skillnum)
{
	// 마법사 클래스가 아니라면, return
	if ( PlayerClass != EPlayerClassType::Mage ) {
		return;
	}

	switch ( skillnum ) {
	case 0: { Montage_Play(MageSkill01); } break;
	case 1: { Montage_Play(MageSkill02); } break;
	default: { UE_LOG(LogTemp , Warning , TEXT("No Anim Montage Playing")); }; break;
	}
}

void UPlayerAnimInstance::AnimNotify_AN_MageSkill01()
{
	//전기 스킬
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if ( !Player )
	{
		UE_LOG(LogTemp , Warning , TEXT("AnimNotify_AN_MageSkill01: 플레이어 캐릭터를 찾을 수 없습니다."));
		return;
	}

	UMageSkill* mage = Cast<UMageSkill>(Player->PlayerSkillComp->SkillStrategy);

	if ( mage )
	{
		mage->ElectroSkill(Player);
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("AnimNotify_AN_MageSkill01: MageSkill이 설정되어 있지 않습니다."));
	}
}

void UPlayerAnimInstance::AnimNotify_AN_MageSkill01_End()
{
	// 스킬이 끝나면 스킬 끝내기
	OwnerCharacter->bUsingSkill = false;
	OwnerCharacter->bSkillEnded = true;
}

void UPlayerAnimInstance::AnimNotify_AN_MageSkill02()
{
	// 불 공(파이어 공격)
	//OwnerCharacter->PlayerSkillComp->UseSkill(1);
}

void UPlayerAnimInstance::AnimNotify_AN_MageSkill02_End()
{
	// 스킬이 끝나면 스킬 끝내기
	OwnerCharacter->bUsingSkill = false;
	OwnerCharacter->bSkillEnded = true;

	//UE_LOG(LogTemp, Warning, "Skill using is false, skill ended is true");
}

//==============================================================================
// 플레이어 데미지(피격) 애니메이션
void UPlayerAnimInstance::PlayDamagedAnimation()
{
	Montage_Play(PlayerDamaged);
}

// 플레이어 죽음 애니메이션 재생
void UPlayerAnimInstance::PlayDeadAnimation()
{
	Montage_Play(PlayerDead);
}

void UPlayerAnimInstance::AnimNotify_ShowDeathUI()
{
	if ( OwnerCharacter && OwnerCharacter->IsLocallyControlled() ) {
		OwnerCharacter->bShowDeathUI = true;
		OwnerCharacter->OnDeathAnimationFinished();
	}
}

//==============================================================================