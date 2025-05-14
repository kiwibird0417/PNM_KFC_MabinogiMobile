// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSkillUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerSkillUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


//---------------------------------------------------
public:
    // 1. 스킬 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Skill01;  

	UFUNCTION()
	void OnSkill01ButtonClicked();


	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Skill02;  
	
	UFUNCTION()
	void OnSkill02ButtonClicked();

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Skill03;
	
	UFUNCTION()
	void OnSkill03ButtonClicked();

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Skill04;  

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Skill05;  

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Skill06;  

	//---------------------------------------------------
	// 2. 버튼 이미지
	// 기본 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default") 
	UTexture2D* DefaultEmptyIcon;

	void SetSkillIcon(int32 Index , UTexture2D* SkillIcon , bool bHasSkill);

	//---------------------------------------------------
	// 3. Skill 이름
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName01;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName02;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName03;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName04;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName05;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName06;


	//---------------------------------------------------
	// 3. 할당해주기
	// Data Table을 불러와서, Skill을 할당해주고, UI에 보여주기
	UFUNCTION(BlueprintCallable)
	void SetSkillNameText(int32 Index, const FText& SkillName);



//---------------------------------------------------
//4. 스킬 쿨타임 구현해보기
	// 버튼 쓸 수 없게 막기 --> isUsingSkill일 떄, false로 enable 버튼을 끈다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanUseSkillButton = true;

	//---------------------------------------------------
public:
	// 헤더	
	// 쿨타임 관련 변수
	FTimerHandle CooldownTimerHandle;

	float SkillCooldown = 6.0f;
	float SkillElapsed = 0.0f;
	bool bSkillOnCooldown = false;

	// 쿨타임 관련 변수 (Skill 2)
	FTimerHandle CooldownTimerHandle02;
	float SkillCooldown02 = 10.0f;
	float SkillElapsed02 = 0.0f;
	bool bSkillOnCooldown02 = false;

	// 쿨타임 관련 변수 (Skill 3)
	FTimerHandle CooldownTimerHandle03;
	float SkillCooldown03 = 8.0f;
	float SkillElapsed03 = 0.0f;
	bool bSkillOnCooldown03 = false;


	//=========================================================================
	// 함수 선언
	void StartCooldown();
	void UpdateCooldownByTimer();
	void ResetCooldown();

	void UpdateCooldown(float Percent , float RemainingTime);

	void StartCooldown02();
	void UpdateCooldownByTimer02();
	void ResetCooldown02();
	void UpdateCooldown02(float Percent , float RemainingTime);

	void StartCooldown03();
	void UpdateCooldownByTimer03();
	void ResetCooldown03();
	void UpdateCooldown03(float Percent , float RemainingTime);





	//==========================================================================
	// ProgressBar와 Text는 위젯에서 만든 변수 이름과 일치시켜야 함
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Skill01;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillCooldownText01;

	// Skill 2
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Skill02;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillCooldownText02;
	
	// Skill 3
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Skill03;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillCooldownText03;






	
};
