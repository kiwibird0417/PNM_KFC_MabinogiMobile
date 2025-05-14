// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerInventory/WeaponItemUI.h"

#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UWeaponItemUI::NativeConstruct()
{
	Super::NativeConstruct();

	if ( WeaponButton )
	{
		WeaponButton->OnClicked.AddDynamic(this , &UWeaponItemUI::OnClicked);
	}
	
}

//-------------------------------------------------------------------
void UWeaponItemUI::Setup(UWeaponInstance* InInstance)
{
	WeaponInstance = InInstance;

	if ( WeaponInstance && WeaponIcon )
	{
		WeaponIcon->SetBrushFromTexture(WeaponInstance->GetWeaponImage());

		FVector2D Size = WeaponIcon->Brush.ImageSize;
		UE_LOG(LogTemp , Warning , TEXT("WeaponIcon Size: %f x %f") , Size.X , Size.Y);

	}
}

//-------------------------------------------------------------------
