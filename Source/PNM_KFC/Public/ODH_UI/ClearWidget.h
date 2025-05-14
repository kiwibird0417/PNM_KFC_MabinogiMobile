// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_exit;

	UFUNCTION()
	void OnExit();

public:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim ))
	class UWidgetAnimation* ClearAnimation;

	void PlayUIAnim();
};
