// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerInfo/PlayerInfoDetailsUI.h"
#include "AHS_Player/PlayerCharacter.h"
#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "Components/Button.h"
#include "AHS_PlayerComponent/AC_PlayerInputComponent.h"

void UPlayerInfoDetailsUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 1. 플레이어 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());


	// 2. 클래스 변경 관련 버튼 연결
	if ( Button_Warrior )
	{
		Button_Warrior->OnClicked.AddDynamic(this , &UPlayerInfoDetailsUI::OnClickWarriorButton);
	}

	if ( Button_Archer ) {
		Button_Archer->OnClicked.AddDynamic(this , &UPlayerInfoDetailsUI::OnClickArcherButton);
	}

	if ( Button_Mage ) {
		Button_Mage->OnClicked.AddDynamic(this , &UPlayerInfoDetailsUI::OnClickMageButton);
	}

	if ( Button_Healer ) {
		Button_Healer->OnClicked.AddDynamic(this , &UPlayerInfoDetailsUI::OnClickHealerButton);
	}

	if ( Button_Bard ) {
		Button_Bard->OnClicked.AddDynamic(this , &UPlayerInfoDetailsUI::OnClickBardButton);
	}


	//-----------------------------------------------------------------------
	// 3. 닫기
	if ( Button_CloseWindow ) {
		Button_CloseWindow->OnClicked.AddDynamic(this , &UPlayerInfoDetailsUI::OnClickedCloseUIWindow);
	}


}

//-----------------------------------------------------------------------

void UPlayerInfoDetailsUI::OnClickWarriorButton()
{
	OnChangePlayerClass(EPlayerClassType::Warrior);
}

void UPlayerInfoDetailsUI::OnClickArcherButton()
{
	OnChangePlayerClass(EPlayerClassType::Archer);
}

void UPlayerInfoDetailsUI::OnClickMageButton()
{
	OnChangePlayerClass(EPlayerClassType::Mage);
}

void UPlayerInfoDetailsUI::OnClickHealerButton()
{
	OnChangePlayerClass(EPlayerClassType::Healer);
}

void UPlayerInfoDetailsUI::OnClickBardButton()
{
	OnChangePlayerClass(EPlayerClassType::Bard);
}

// 플레이어 클래스 변경
void UPlayerInfoDetailsUI::OnChangePlayerClass(EPlayerClassType PlayerClass)
{
	// 이전과 클래스가 같다면 변경하지 않는다.
	if ( OwnerCharacter->PlayerClassComp->PlayerClass == PlayerClass ) {
		return;
	}

	//------------------------------------------------------------
	// 새로운 클래스로 업데이트
	OwnerCharacter->PlayerClassComp->PlayerClass = PlayerClass;
	OwnerCharacter->PlayerClassComp->SetupClassDefaults();
}

//--------------------------------------------------------------------
// 창 닫기
void UPlayerInfoDetailsUI::OnClickedCloseUIWindow()
{
	OwnerCharacter->PlayerInputComp->CloseInfoMenu();
}


//----------------------------------------------------------------------
