// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerInventory/WeaponDetailsUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"
#include "AHS_Player/PlayerCharacter.h"
#include "Components/Button.h"
#include "AHS_UI/PlayerInfo/PlayerInventoryUI.h"

void UWeaponDetailsUI::NativeConstruct()
{
	// 1. 플레이어 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());


	// 1-1. 상호작용 버튼 연결
	if ( Button_Equip )
	{
		Button_Equip->OnClicked.AddDynamic(this , &UWeaponDetailsUI::EquipWeapon);
	}
}

//-------------------------------------------------------------
void UWeaponDetailsUI::Setup(UWeaponInstance* Instance , UPlayerInventoryUI* InParentUI)
{
	WeaponInstance = Instance;
	ParentInventoryUI = InParentUI;

	if ( Instance )
	{
		WeaponNameText->SetText(FText::FromString(Instance->GetWeaponName().ToString()));
		DescriptionText->SetText(FText::FromString(Instance->GetWeaponDescription().ToString()));
		WeaponImage->SetBrushFromTexture(Instance->GetWeaponImage());
	}
}


//-------------------------------------------------------------
// 배경에 입력 누르면 닫기
FReply UWeaponDetailsUI::NativeOnMouseButtonDown(const FGeometry& InGeometry , const FPointerEvent& InMouseEvent)
{
	if ( InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) )
	{
		FVector2D MousePos = InMouseEvent.GetScreenSpacePosition();

		const bool bIsOverDetailBG = DetailBG && DetailBG->GetCachedGeometry().IsUnderLocation(MousePos);
		const bool bIsOverDetailImgBG = DetailImg_BG && DetailImg_BG->GetCachedGeometry().IsUnderLocation(MousePos);

		// 만약 마우스 클릭이 두 배경 중 하나 위에 있다면, UI 닫기
		if ( bIsOverDetailBG || bIsOverDetailImgBG )
		{
			RemoveFromParent();
			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry , InMouseEvent);
}

//-------------------------------------------------------------
// 무기 장착하기
void UWeaponDetailsUI::EquipWeapon()
{
	if ( OwnerCharacter && WeaponInstance )
	{
		OwnerCharacter->EquipWeapon(WeaponInstance);

		// UI 갱신
		if ( ParentInventoryUI && ParentInventoryUI->EquippedWeaponImage && WeaponInstance->GetWeaponImage() )
		{
			ParentInventoryUI->EquippedWeaponImage->SetBrushFromTexture(WeaponInstance->GetWeaponImage());
		}

		RemoveFromParent();
	}
}



//-------------------------------------------------------------
