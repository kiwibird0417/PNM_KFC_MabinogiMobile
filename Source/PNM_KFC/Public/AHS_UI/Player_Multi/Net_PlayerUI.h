// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Net_PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UNet_PlayerUI : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;

//------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HP)
	class UTextBlock* txt_nickName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HP)
	float HP = 1.0f;
	

	// 이름을 UI에 설정하는 메서드 추가
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetPlayerName(const FString& name);

};
