// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHS_Interactables/AC_InteractableComponent.h"
#include "AC_EnemyInteractableComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PNM_KFC_API UAC_EnemyInteractableComponent : public UAC_InteractableComponent
{
	GENERATED_BODY()

public:
	UAC_EnemyInteractableComponent();

public:
	virtual bool IsEnemy() const override { return true; }
	
};
