// 플레이어 입력 관련 Component

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "AC_PlayerInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UAC_PlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_PlayerInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInputComponent(APlayerController* PlayerController, UInputComponent* PlayerInputComponent);

//----------------------------------------------------------
public:
	// 플레이어 캐릭터 참조
	class APlayerCharacter* OwnerCharacter;

	//---------------------------------------------------
	//Input Action 관련
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* IMC_LocalPlayer;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_DebugTest;

	void DebugTest();

	//---------------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_CameraRotation;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_CameraZoom;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_InterAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_LeftMouseClick;

	//--------------------------------------------------
	// 스킬 input action 연결
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerSkill01;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerSkill02;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerSkill03;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerSkill04;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerSkill05;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerSkill06;

	//--------------------------------------------------
	// 플레이어 스킬 함수
	void UseSkill01();
	void UseSkill02();
	void UseSkill03();
	void UseSkill04();
	void UseSkill05();
	void UseSkill06();

	//--------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_PlayerInfo;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Inventory;



//------------------------------------------------------------
	// 1. 일반 이동 구현
	void Move(const FInputActionValue& Value);


	// 2. 플레이어 카메라 회전 구현
	void Look(const FInputActionValue& Value);

	void Zoom(const FInputActionValue& Value);

	// 3. 상호작용
	void LeftMouseClick(const FInputActionValue& Value);

	void InterAction(const FInputActionValue& Value);


//------------------------------------------------------------
// UI Menu 관련
	// 1. 플레이어 정보 (PlayerInfo)
	void OpenPlayerInfoMenu(const FInputActionValue& Value);

	// 플레이어 정보 UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerInfoDetailsUI> detailsUIWidget;

	UPROPERTY()
	class UPlayerInfoDetailsUI* detailsUI;

	bool isShowingDetailUI = false;

	void ShowAndHideDetailUI();
	void CloseInfoMenu();

//------------------------------------------------------------
//2. 인벤토리 관련
// 디버깅 테스트용으로 임시 연결
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_DebugTest1;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_DebugTest2;

	void DebugTest_AddWeapon(const FInputActionValue& Value);
	void DebugTest_PrintInventoryWeapon(const FInputActionValue& Value);

	// 인벤토리 UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerInventoryUI> inventoryUIWidget;

	UPROPERTY()
	class UPlayerInventoryUI* inventoryUI;

	void OpenPlayerInventoryMenu(const FInputActionValue& Value);


	bool isShowingInventoryUI = false;

	//------------------------------------------------------------------------
	// 인벤토리 열기
	void ShowAndHideInventoryUI();
	void CloseInventoryMenu();

//-------------------------------------------------------------------------------
//2-1. 인벤토리 관련... 일단 퀵슬롯 임시 구현(포션, HP 회복 + 부활 아이템, 0415~0416(화/수 기준))
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_QuickSlot07;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_QuickSlot08;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_QuickSlot09;

	void QuickSlot07(const FInputActionValue& Value);
	void QuickSlot08(const FInputActionValue& Value);
	void QuickSlot09(const FInputActionValue& Value);


//-------------------------------------------------------------------------------
//0428(금)
// 마우스 커서 보이도록 설정하기 ( + Alt 키)
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_ShowMouseCursorAlt;

	void ShowMouseCursorAlt(const FInputActionValue& Value);

	bool bShowMouseCursorAlt = false;
		
};
