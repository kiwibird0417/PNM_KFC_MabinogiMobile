// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AHS_Player/AHS_PlayerInventory/FInventoryItem.h"

#include "AC_PlayerInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PNM_KFC_API UAC_PlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_PlayerInventoryComponent();

//------------------------------------------------------------------------------------
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSoftObjectPtr<UDataTable> WeaponDataTableAsset;

	UDataTable* WeaponDataTable = nullptr;



//------------------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> InventoryItems;

	public:
	// 무기 획득 시 호출
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddWeaponInstance(class UWeaponInstance* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UWeaponInstance*> GetAllWeapons();

	

};