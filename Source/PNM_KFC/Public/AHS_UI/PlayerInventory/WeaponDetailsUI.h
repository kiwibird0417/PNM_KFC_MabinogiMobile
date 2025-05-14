// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponDetailsUI.generated.h"

/**
 * 
 */



UCLASS()
class PNM_KFC_API UWeaponDetailsUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

//------------------------------------------------------
	UPROPERTY()
	class UPlayerInventoryUI* ParentInventoryUI;

	UPROPERTY()
	class APlayerCharacter* OwnerCharacter;

//-------------------------------------------------------------------
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponImage;

	UFUNCTION(BlueprintCallable)
	void Setup(UWeaponInstance* Instance , UPlayerInventoryUI* InParentUI);

	//-----------------------------------------------------
	// 배경 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* DetailBG;

	UPROPERTY(meta = (BindWidget))
	class UImage* DetailImg_BG;

	//-----------------------------------------------------
	// 무기 장착하기 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Equip;  

	UFUNCTION()
	void EquipWeapon();

private:
	UPROPERTY()
	UWeaponInstance* WeaponInstance;


//-------------------------------------------------------------------
public:
	// 배경을 누르면 닫힌다.
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry , const FPointerEvent& InMouseEvent) override;


};
