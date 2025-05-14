// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UAC_InteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_InteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//------------------------------------------------------------------------
public:
	// 플레이어 캐릭터 참조
	class APlayerCharacter* LocalPlayer;

	// 이 액터 컴포넌트의 부모 클래스 찾기
	class AActor* OwnerActor;


	// 에너미인지 아닌지 판별
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Actor Type")
	bool bIsEnemy = false;

	virtual bool IsEnemy() const { return bIsEnemy; }

//----------------------------------------------------------
public: 
	// 현재 타겟 가능한 상태인지 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Auto Targeting")
    bool bCanTarget = true;

    // 나중에 타겟 해제하거나 처리할 이벤트
    UFUNCTION(BlueprintCallable, Category="Auto Targeting")
    void SetTargetable(bool bTargetable);

	//----------------------------------------------------------
	// UI 생성해서 붙여주기

	UPROPERTY()
	class UWidgetComponent* InteractableWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UInteractableUI> mainUIWidget;
	
	UPROPERTY()
	class UInteractableUI* mainUI;


	// 생성한 UI 보여주기
	void ShowUI();

	void HideUI();

	bool bIsUIVisible = false;
//----------------------------------------------------------

		
};
