// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDeadUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerDeadUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRetry();

	UFUNCTION()
	void OnExit();

//=====================================================
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Resurrect;

	UPROPERTY(meta = (BindWidget))
    class UButton* Button_LeaveRoom;
	

	
};
