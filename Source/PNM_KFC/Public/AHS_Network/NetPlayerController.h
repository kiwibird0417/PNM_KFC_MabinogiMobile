// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API ANetPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class APNM_KFCGameMode* gm;

public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	//------------------------------------------------
	// Player Controller가 Main UI 소유
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerMainUI> mainUIWidget;

	UPROPERTY()
	class UPlayerMainUI* mainUI;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerInfoUI> infoUIWidget;

	UPROPERTY()
	class UPlayerInfoUI* infoUI;

	//------------------------------------------------
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpectator();

public:
	FString playerName;

public:
    UFUNCTION(Client, Reliable)
    void ClientUpdatePlayerName(const FString& name);
	

public:
	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowClearUI();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClearUI;
};
