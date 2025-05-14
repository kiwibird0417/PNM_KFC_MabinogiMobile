// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoDetailsUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerInfoDetailsUI : public UUserWidget
{
	GENERATED_BODY()

//-----------------------------------------------------------------
public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickWarriorButton();

	UFUNCTION()
	void OnClickArcherButton();
	
	UFUNCTION()
	void OnClickMageButton();
	
	UFUNCTION()
	void OnClickHealerButton();
	
	UFUNCTION()
	void OnClickBardButton();


	UFUNCTION()
    void OnChangePlayerClass(EPlayerClassType PlayerClass);

//------------------------------------------------------
// 플레이어 참조
	UPROPERTY()
	class APlayerCharacter* OwnerCharacter;

//-----------------------------------------------------------------
// 1. 플레이어 클래스 변경 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Warrior;  

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Archer;  

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Mage;  

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Healer;  

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_Bard;  

//-----------------------------------------------------------------
// 2. 디테일 창 닫기
	
	UPROPERTY(meta = (BindWidget))
    class UButton* Button_CloseWindow;  

	UFUNCTION()
	void OnClickedCloseUIWindow();

//-----------------------------------------------------------------

	
};
