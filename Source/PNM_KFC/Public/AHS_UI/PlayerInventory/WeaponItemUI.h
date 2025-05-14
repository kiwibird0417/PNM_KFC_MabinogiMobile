// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "WeaponItemUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponItemClicked , UWeaponInstance* , WeaponInstance);


UCLASS()
class PNM_KFC_API UWeaponItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// 버튼 추가 (UMG에서도 위에 배치)
	UPROPERTY(meta = (BindWidget))
	class UButton* WeaponButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponIcon;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnWeaponItemClicked OnWeaponItemClicked;

	UPROPERTY()
	class UWeaponInstance* WeaponInstance;

protected:
	virtual void NativeConstruct() override;

public:
	// 초기 설정 함수
	UFUNCTION(BlueprintCallable)
	void Setup(UWeaponInstance* InInstance);


	UFUNCTION()
	void OnClicked()
	{
		OnWeaponItemClicked.Broadcast(WeaponInstance);
	}
};
