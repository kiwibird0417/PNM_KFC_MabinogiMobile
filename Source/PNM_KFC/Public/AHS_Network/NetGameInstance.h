﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"

#include "NetGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString playerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingSpeed;
	UPROPERTY(BlueprintReadOnly)
	int32 index;

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s, %dms"), index, *roomName, *hostName, *playerCount, pingSpeed);
	}
};

// 세션검색 끝났을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature , const FSessionInfo& , sessionInfo);

// 세션검색 상태 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature , bool , bIsSearching);



UCLASS()
class PNM_KFC_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

//=========================================================
//0423(수)
public: 
	virtual void Init() override;

public:
	IOnlineSessionPtr sessionInterface;

	void CreateMySession(FString roomName, int32 playerCount);

	// 세션 호스트 이름
	FString mySessionName = "각자 이름(영어로)";

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//=========================================================
	// 방 검색
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	void FindOtherSession();

	void OnFindSessionsComplete(bool bWasSuccessful);

	//=========================================================
	// 다국어 인코딩
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);

	//=========================================================
	// 방찾기완료 콜백을 등록할 델리게이트
	FSearchSignature onSearchCompleted;

	// 방찾기상태 콜백 델리게이트
	FSearchStateSignature onSearchState;

	//=========================================================
	// 세션(방) 입장
	void JoinSelectedSession(int32 index);

	// 세션입장 콜백
	void OnJoinSessionCompleted(FName sessionName , EOnJoinSessionCompleteResult::Type result);

	//=========================================================
	// 세션(방) 퇴장
	void ExitRoom();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ExitRoom();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ExitRoom();

	void OnMyExitRoomComplete(FName sessionName, bool bWasSuccessful);


	bool IsInRoom();
};
