// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerInventoryUI : public UUserWidget
{
	GENERATED_BODY()

//----------------------------------------------------------------
public:
	// 무기 아이템 UI 생성 함수
	void PopulateWeaponInventory(const TArray<class UWeaponInstance*>& WeaponList);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UniformGridPanel_WeaponItems;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UWeaponItemUI> WeaponItemClass;

private:
	const int32 Columns = 5;

//-------------------------------------------------------------------
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<class UWeaponDetailsUI> WeaponDetailsUIClass;

	UPROPERTY()
	UWeaponDetailsUI* CurrentWeaponDetailsUI;

	UFUNCTION()
	void OnWeaponItemClicked(UWeaponInstance* ClickedWeapon);

//--------------------------------------------------------------------
// 장착하고 있는 무기 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* EquippedWeaponImage;

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_CloseMenu;  

	UFUNCTION()
	void OnClickedCloseUIWindow();


	
};
