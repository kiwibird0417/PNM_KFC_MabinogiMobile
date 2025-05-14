// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UEnemyUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "HP")
	float hp = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Name)
	FString Name;
};
