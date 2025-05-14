// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

    UFUNCTION()
    void OnInterActionButtonClicked();
//------------------------------------------------------
	UPROPERTY()
	class APlayerCharacter* OwnerCharacter;


//-------------------------------------------------------
public:
    // 1. 상호작용 버튼(스페이스 바)
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Interaction;  

	// 1-1. 상호작용 버튼 -> 이미지 변경
	//UPROPERTY(meta = (BindWidget))
	//class UImage* Image_Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UTexture2D* Texture_Auto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UTexture2D* Texture_Manual;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UTexture2D* Texture_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UTexture2D* Texture_Normal_Interaction;

	void ChangeInterActionButtonImage(bool bIsAuto);

//-------------------------------------------------------
public:
    // 2. 인벤토리 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Inventory;  

	UFUNCTION()
	void OnClickInventoryButton();


//-------------------------------------------------------
public:

	// 네트워크 세팅
	void SetOwnerCharacter(APlayerCharacter* InCharacter);

};
