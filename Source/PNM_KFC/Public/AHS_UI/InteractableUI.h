// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UInteractableUI : public UUserWidget
{
	GENERATED_BODY()

//-------------------------------------------------------
public:
	//UI 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeAnim;

	void PlayFadeIn();
	void PlayFadeOut();

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Interactable;
	
};
