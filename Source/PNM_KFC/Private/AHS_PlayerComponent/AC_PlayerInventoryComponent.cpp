// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_PlayerComponent/AC_PlayerInventoryComponent.h"
#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"

// Sets default values for this component's properties
UAC_PlayerInventoryComponent::UAC_PlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	WeaponDataTableAsset = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/AHS/Data/WeaponDataTables/DT_WeaponData.DT_WeaponData'"))
	);
}

void UAC_PlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if ( WeaponDataTableAsset.IsValid() )
	{
		WeaponDataTable = WeaponDataTableAsset.Get();
	}
	else
	{
		WeaponDataTable = WeaponDataTableAsset.LoadSynchronous();
	}
}

//-----------------------------------------------------------------------------------------------

void UAC_PlayerInventoryComponent::AddWeaponInstance(class UWeaponInstance* NewWeapon)
{
	if ( !NewWeapon ) return;

	FInventoryItem NewItem;
	NewItem.Category = EInventoryCategory::Equipment;
	NewItem.ItemInstance = NewWeapon;

	InventoryItems.Add(NewItem);
}

TArray<UWeaponInstance*> UAC_PlayerInventoryComponent::GetAllWeapons()
{
	TArray<UWeaponInstance*> Weapons;

	for ( const FInventoryItem& Item : InventoryItems )
	{
		if ( Item.Category == EInventoryCategory::Equipment )
		{
			if ( UWeaponInstance* Weapon = Cast<UWeaponInstance>(Item.ItemInstance) )
			{
				Weapons.Add(Weapon);
			}
		}
	}
	return Weapons;
}

