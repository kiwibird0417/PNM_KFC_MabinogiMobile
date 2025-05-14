// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

//=========================================================
//0423(수)
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_createRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_roomName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class USlider* slider_playerCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_playerCount;

	UPROPERTY()
	class UNetGameInstance* GI;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateRoom();

	UFUNCTION()
	void OnValueChanged(float Value);

	//=========================================================
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Lobby;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_createSession;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_findSession;

	UFUNCTION()
	void SwitchCreatePanel();

	UFUNCTION()
	void SwitchFindPanel();

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_back;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_back_1;

	UFUNCTION()
	void BackToMain();

	//==========================================================
	// 세션 슬롯
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UScrollBox* Scroll_RoomList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USessionSlotWidget> SessionInfoWidget;

	UFUNCTION()
	void AddSlotWidget(const struct FSessionInfo& InSessionInfo);

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* btn_find;

	// 방찾기 버튼 클릭시 호출될 콜백
	UFUNCTION()
	void OnClickedFindSession();

	//=========================================================================
	// 검색중 메시지
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* txt_findingMsg;

	// 방찾기 상태 이벤트 콜백
	UFUNCTION()
	void OnChangeButtonEnable(bool bIsSearching);


	//=========================================================================
	// 플레이어 이름 입력
	UPROPERTY(BlueprintReadOnly , meta = (BindWidget))
	class UEditableText* edit_userName;


	
};
