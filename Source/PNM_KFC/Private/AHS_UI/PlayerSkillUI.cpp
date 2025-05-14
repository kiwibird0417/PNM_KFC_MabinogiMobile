// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerSkillUI.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

void UPlayerSkillUI::NativeConstruct()
{
	Super::NativeConstruct();

	SkillCooldown = 6.0f;
	SkillElapsed = 0.0f;
	bSkillOnCooldown = false;

	if ( ProgressBar_Skill01 )
	{
		ProgressBar_Skill01->SetPercent(0.0f);
	}

	if ( SkillCooldownText01 )
	{
		SkillCooldownText01->SetText(FText::GetEmpty());
	}

	if ( ProgressBar_Skill02 )
	{
		ProgressBar_Skill02->SetPercent(0.0f);
	}

	if ( SkillCooldownText02 )
	{
		SkillCooldownText02->SetText(FText::GetEmpty());
	}

	if ( ProgressBar_Skill03 )
	{
		ProgressBar_Skill03->SetPercent(0.0f);
	}

	if ( SkillCooldownText03 )
	{
		SkillCooldownText03->SetText(FText::GetEmpty());
	}


	//===========================================================
	if ( Button_Skill01 )
	{
		Button_Skill01->OnClicked.AddDynamic(this , &UPlayerSkillUI::OnSkill01ButtonClicked);
	}
	if ( Button_Skill02 )
	{
		Button_Skill02->OnClicked.AddDynamic(this , &UPlayerSkillUI::OnSkill02ButtonClicked);
	}
	if ( Button_Skill03 )
	{
		Button_Skill03->OnClicked.AddDynamic(this , &UPlayerSkillUI::OnSkill03ButtonClicked);
	}

}



void UPlayerSkillUI::OnSkill01ButtonClicked()
{

}

void UPlayerSkillUI::OnSkill02ButtonClicked()
{
	if ( !bSkillOnCooldown02 )
	{
		StartCooldown02();
	}
}

void UPlayerSkillUI::OnSkill03ButtonClicked()
{

}

//=======================================================================
// 스킬 버튼 1 타이머(스킬 쿨타임 적용)
void UPlayerSkillUI::StartCooldown()
{
	if ( bSkillOnCooldown ) return;

	// Enable Button 비활성화
	bCanUseSkillButton = false;


	//---------------------------------------
	bSkillOnCooldown = true;
	SkillElapsed = 0.0f;

	// 타이머 시작 (0.1초 간격)
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle , this , &UPlayerSkillUI::UpdateCooldownByTimer , 0.1f , true);
}

void UPlayerSkillUI::UpdateCooldownByTimer()
{
	SkillElapsed += 0.1f;
	float Percent = SkillElapsed / SkillCooldown;
	float Remaining = FMath::Max(0.0f , SkillCooldown - SkillElapsed);

	UpdateCooldown(Percent , Remaining);

	if ( SkillElapsed >= SkillCooldown )
	{
		// 쿨타임 종료
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		bSkillOnCooldown = false;

		// 버튼 누를 수 있게 하기
		bCanUseSkillButton = true;
		ResetCooldown();
	}
}

void UPlayerSkillUI::UpdateCooldown(float Percent , float RemainingTime)
{
	if ( ProgressBar_Skill01 )
	{
		ProgressBar_Skill01->SetPercent(Percent);
	}

	if ( SkillCooldownText01 )
	{
		FString TimeString = FString::Printf(TEXT("%.1f") , RemainingTime);
		SkillCooldownText01->SetText(FText::FromString(TimeString));
	}
}

void UPlayerSkillUI::ResetCooldown()
{
	if ( ProgressBar_Skill01 )
	{
		ProgressBar_Skill01->SetPercent(0.0f);
	}

	if ( SkillCooldownText01 )
	{
		SkillCooldownText01->SetText(FText::GetEmpty());
	}
}
//=======================================================================
// 스킬 버튼 2 타이머(스킬 쿨타임 적용)
void UPlayerSkillUI::StartCooldown02()
{
	if ( bSkillOnCooldown02 ) return;

	// Enable Button 비활성화
	bCanUseSkillButton = false;


	//---------------------------------------
	bSkillOnCooldown02 = true;
	SkillElapsed02 = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle02 , this , &UPlayerSkillUI::UpdateCooldownByTimer02 , 0.1f , true);
}

void UPlayerSkillUI::UpdateCooldownByTimer02()
{
	SkillElapsed02 += 0.1f;
	float Percent = SkillElapsed02 / SkillCooldown02;
	float Remaining = FMath::Max(0.0f , SkillCooldown02 - SkillElapsed02);

	UpdateCooldown02(Percent , Remaining);

	if ( SkillElapsed02 >= SkillCooldown02 )
	{
		// 쿨타임 종료
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle02);
		bSkillOnCooldown02 = false;

		// 버튼 누를 수 있게 하기
		bCanUseSkillButton = true;
		ResetCooldown02();
	}
}

void UPlayerSkillUI::UpdateCooldown02(float Percent , float RemainingTime)
{
	if ( ProgressBar_Skill02 )
	{
		ProgressBar_Skill02->SetPercent(Percent);
	}
	if ( SkillCooldownText02 )
	{
		SkillCooldownText02->SetText(FText::FromString(FString::Printf(TEXT("%.1f") , RemainingTime)));
	}
}

void UPlayerSkillUI::ResetCooldown02()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle02);
	bSkillOnCooldown02 = false;

	if ( ProgressBar_Skill02 )
	{
		ProgressBar_Skill02->SetPercent(0.0f);
	}
	if ( SkillCooldownText02 )
	{
		SkillCooldownText02->SetText(FText::GetEmpty());
	}
}


//=======================================================================
// 스킬 버튼 3 타이머(스킬 쿨타임 적용)
void UPlayerSkillUI::StartCooldown03()
{
	bSkillOnCooldown03 = true;
	SkillElapsed03 = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle03 , this , &UPlayerSkillUI::UpdateCooldownByTimer03 , 0.1f , true);
}

void UPlayerSkillUI::UpdateCooldownByTimer03()
{
	SkillElapsed03 += 0.1f;
	float Percent = SkillElapsed03 / SkillCooldown03;
	float Remaining = SkillCooldown03 - SkillElapsed03;

	UpdateCooldown03(1.0f - Percent , Remaining);

	if ( SkillElapsed03 >= SkillCooldown03 )
	{
		ResetCooldown03();
	}
}

void UPlayerSkillUI::UpdateCooldown03(float Percent , float RemainingTime)
{
	if ( ProgressBar_Skill03 )
	{
		ProgressBar_Skill03->SetPercent(Percent);
	}
	if ( SkillCooldownText03 )
	{
		SkillCooldownText03->SetText(FText::FromString(FString::Printf(TEXT("%.1f") , RemainingTime)));
	}
}

void UPlayerSkillUI::ResetCooldown03()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle03);
	bSkillOnCooldown03 = false;

	if ( ProgressBar_Skill03 )
	{
		ProgressBar_Skill03->SetPercent(0.0f);
	}
	if ( SkillCooldownText03 )
	{
		SkillCooldownText03->SetText(FText::GetEmpty());
	}
}







//=======================================================================
void UPlayerSkillUI::SetSkillNameText(int32 Index , const FText& SkillName)
{
	switch ( Index )
	{
	case 0:
		if ( SkillName01 ) SkillName01->SetText(SkillName);
		break;
	case 1:
		if ( SkillName02 )  SkillName02->SetText(SkillName);
		break;
	case 2:
		if ( SkillName03 )  SkillName03->SetText(SkillName);
		break;
	case 3:
		if ( SkillName04 ) SkillName04->SetText(SkillName);
		break;
	case 4:
		if ( SkillName05 ) SkillName05->SetText(SkillName);
		break;
	case 5:
		if ( SkillName06 ) SkillName06->SetText(SkillName);
		break;
	default:
		UE_LOG(LogTemp , Log , TEXT("No Update"));
		break;
	}
}


void UPlayerSkillUI::SetSkillIcon(int32 Index , UTexture2D* SkillIcon , bool bHasSkill)
{
	UButton* TargetButton = nullptr;

	switch ( Index )
	{
	case 0: TargetButton = Button_Skill01; break;
	case 1: TargetButton = Button_Skill02; break;
	case 2: TargetButton = Button_Skill03; break;
	case 3: TargetButton = Button_Skill04; break;
	case 4: TargetButton = Button_Skill05; break;
	case 5: TargetButton = Button_Skill06; break;
	default: return;
	}

	if ( !TargetButton ) return;

	// 기존 스타일 가져오기
	FButtonStyle ButtonStyle = TargetButton->WidgetStyle;

	// 브러시 생성
	FSlateBrush SkillBrush;
	SkillBrush.SetResourceObject(bHasSkill && SkillIcon ? SkillIcon : DefaultEmptyIcon);
	SkillBrush.ImageSize = FVector2D(64.f , 64.f); // 아이콘 크기 설정

	// 버튼 상태에 따라 동일한 브러시 적용
	ButtonStyle.Normal = SkillBrush;
	ButtonStyle.Hovered = SkillBrush;
	ButtonStyle.Pressed = SkillBrush;

	// 버튼 스타일 갱신
	TargetButton->SetStyle(ButtonStyle);

	// 활성화/비활성화
	TargetButton->SetIsEnabled(bHasSkill);
}

//=======================================================================


