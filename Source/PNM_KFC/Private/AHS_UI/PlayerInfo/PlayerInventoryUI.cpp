// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerInfo/PlayerInventoryUI.h"
#include "AHS_UI/PlayerInventory/WeaponItemUI.h"
#include "Components/UniformGridPanel.h"
#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"
#include "Components/UniformGridSlot.h"
#include "AHS_UI/PlayerInventory/WeaponDetailsUI.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "AHS_Player/PlayerCharacter.h"
#include "AHS_PlayerComponent/AC_PlayerInputComponent.h"

void UPlayerInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 3. 닫기
	if ( Button_CloseMenu ) {
		Button_CloseMenu->OnClicked.AddDynamic(this , &UPlayerInventoryUI::OnClickedCloseUIWindow);
	}

}

//-----------------------------------------------------
// 소유하고 있는 무기들 반영
void UPlayerInventoryUI::PopulateWeaponInventory(const TArray<class UWeaponInstance*>& WeaponList)
{

	if ( !WeaponItemClass || !UniformGridPanel_WeaponItems ) return;

	UniformGridPanel_WeaponItems->ClearChildren();

	for ( int32 Index = 0; Index < WeaponList.Num(); ++Index )
	{
		UWeaponItemUI* NewItem = CreateWidget<UWeaponItemUI>(this , WeaponItemClass);


		NewItem->Setup(WeaponList[Index]);

		int32 Row = Index / Columns;
		int32 Col = Index % Columns;




		UUniformGridSlot* GridSlot = UniformGridPanel_WeaponItems->AddChildToUniformGrid(NewItem , Row , Col);
		GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);

		NewItem->OnWeaponItemClicked.AddDynamic(this , &UPlayerInventoryUI::OnWeaponItemClicked);
	}
}

//--------------------------------------------------------
void UPlayerInventoryUI::OnWeaponItemClicked(UWeaponInstance* ClickedWeapon)
{
	if ( !WeaponDetailsUIClass || !ClickedWeapon ) return;

	if ( CurrentWeaponDetailsUI )
	{
		CurrentWeaponDetailsUI->RemoveFromParent();
		CurrentWeaponDetailsUI = nullptr;
	}

	CurrentWeaponDetailsUI = CreateWidget<UWeaponDetailsUI>(this , WeaponDetailsUIClass);
	if ( CurrentWeaponDetailsUI )
	{
		CurrentWeaponDetailsUI->Setup(ClickedWeapon, this);
		CurrentWeaponDetailsUI->AddToViewport();
	}
}

void UPlayerInventoryUI::OnClickedCloseUIWindow()
{
	APawn* OwnerPawn = GetOwningPlayerPawn();
	if ( !OwnerPawn ) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerPawn);
	if ( !PlayerCharacter ) return;

	PlayerCharacter->PlayerInputComp->CloseInventoryMenu();
}




