// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

//================================================================================
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds ) override;

	UPROPERTY()
	class APlayerCharacter* OwnerCharacter;

//================================================================================
// 플레이어 무기(클래스)에 따른 일반 공격 애니메이션 설정
public:
	// 일반 공격 애니메이션 진행
	void Play_BasicAttack_Animation();

	//------------------------------------------------------------
	// 전사 일반 공격
	UPROPERTY(EditDefaultsOnly, Category = Anim)
	class UAnimMontage* WarriorBasicAttackMontage;

	// 전사 일반 공격의 Notify 연결 테스팅 : 애니메이션 노티파이이벤트
	UFUNCTION()
	void AnimNotify_OnWarriorBasicAttackFinish();

	//------------------------------------------------------------
	// 마법사 일반 공격
	UPROPERTY(EditDefaultsOnly, Category = Anim)
	class UAnimMontage* MageBasicAttackMontage;

	// 전사 일반 공격의 Notify 연결 테스팅 : 애니메이션 노티파이이벤트
	UFUNCTION()
	void AnimNotify_OnMageBasicAttackFinish();

//================================================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
	bool bUseAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
	bool bUseSkill = false;

//================================================================================
// 플레이어 클래스 판별
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
	EPlayerClassType PlayerClass;

//================================================================================
// AnimSequence 방식 (현재는 미사용, Anim Montage 사용중)
	// 전사 일반 공격 Anim Sequence Notify 항목
/*
	UFUNCTION()
	void AnimNotify_Warrior_BasicAttack();

	UFUNCTION()
	void AnimNotify_Warrior_BasicAttack_End();

		// 마법사 일반 공격 Anim Sequence Notify 항목
	UFUNCTION()
	void AnimNotify_Mage_BasicAttack();

	UFUNCTION()
	void AnimNotify_Mage_BasicAttack_End();
	*/
//================================================================================

//*******************************************************************************//


//================================================================================
// 플레이어 무기(클래스)에 따른 스킬 공격 애니메이션 설정
public:

	void PlayWarriorSkillAttack(int skillnum);

	// 전사 스킬 공격
	// 연속베기
	UPROPERTY(EditDefaultsOnly, Category = WarriorSkill)
	class UAnimMontage* WarriorSkill01;

	UFUNCTION()
	void AnimNotify_AN_WarriorSkill01_01();

	UFUNCTION()
	void AnimNotify_AN_WarriorSkill01_02();

	UFUNCTION()
	void AnimNotify_AN_WarriorSkill01_03();

	UFUNCTION()
	void AnimNotify_AN_WarriorSkill01_End();

	UFUNCTION()
	void AnimNotify_AN_WarriorSkill02_End();


	//=========================================================================
	UPROPERTY(EditDefaultsOnly, Category = WarriorSkill)
	class UAnimMontage* WarriorSkill02;


	//------------------------------------------------------------
public:
	void PlayMageSkillAttack(int skillnum);

	// 마법사 스킬 공격
	UPROPERTY(EditDefaultsOnly, Category = MageSkill)
	class UAnimMontage* MageSkill01;

	UPROPERTY(EditDefaultsOnly, Category = MageSkill)
	class UAnimMontage* MageSkill02;

	// Anim Notify
	UFUNCTION()
	void AnimNotify_AN_MageSkill01();

	UFUNCTION()
	void AnimNotify_AN_MageSkill01_End();

	UFUNCTION()
	void AnimNotify_AN_MageSkill02();

	UFUNCTION()
	void AnimNotify_AN_MageSkill02_End();
//================================================================================
// 플레이어 피격 및 죽음 구현
public:
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* PlayerDamaged;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* PlayerDead;

	void PlayDamagedAnimation();
	void PlayDeadAnimation();

	UFUNCTION()
	void AnimNotify_ShowDeathUI();

//================================================================================


	
};
