// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API ANetPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

//============================================================
//public:
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Info")
//    FString PlayerName;
//
//    // 클라이언트에서 이름을 설정할 때 사용
//    UFUNCTION(Server, Reliable, WithValidation)
//    void ServerRPC_SetUserName(const FString& Name);
//
//    UFUNCTION(NetMulticast, Reliable)
//    void MulticastRPC_SetUserName(const FString& Name);
//
//    void SetPlayerName(const FString& NewName) { PlayerName = NewName; }
//    FString GetPlayerName() const { return PlayerName; }
//
//    // 추가된 유효성 검사 함수
//    bool ServerRPC_SetUserName_Validate(const FString& Name);


public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Info")
    FString PlayerName;

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerRPC_SetUserName(const FString& Name);

    void SetPlayerName(const FString& NewName) { PlayerName = NewName; }
    FString GetPlayerName() const { return PlayerName; }

    bool ServerRPC_SetUserName_Validate(const FString& Name);

};
