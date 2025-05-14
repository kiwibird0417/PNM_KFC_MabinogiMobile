// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API UPlayerInfoUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

//-------------------------------------------------------
public:
	// 1. 플레이어 체력 (Binding되어 있음)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HP)
	float HP = 1.0f;

	// 2. 플레이어 레벨
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerLevelText;


	UFUNCTION(BlueprintCallable)
	void SetPlayerLevelText(int32 currentPlayerLevel);

	// 3. 플레이어 골드
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerGoldText;

	UFUNCTION(BlueprintCallable)
	void SetPlayerGoldText(int32 currentPlayerGold);

//-------------------------------------------------------
//0425(금)
public:
	// 플레이어 이름 정보 받아오기
	UPROPERTY(meta =(BindWidget))
	class UTextBlock* txt_myName;

	void UpdatePlayerName();

//-------------------------------------------------------
//0428(월)
public:
	// 플레이어 방 나가기 버튼 추가
	UPROPERTY(meta=(BindWidget ))
	class UButton* btn_exit;

	UFUNCTION()
	void OnExit();

	UFUNCTION()
	void SendMsg();

	UFUNCTION()
	void ReceiveMsg(const FString& msg);

// Chatting
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UChatWidget> chatWidget;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox*	scroll_msgList;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableText*	edit_input;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton*	btn_send;



	
};
