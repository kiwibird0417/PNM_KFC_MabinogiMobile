﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget ))
	class UTextBlock* txt_msg;
	
};
