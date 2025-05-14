// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Network/NetGameInstance.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "PNM_KFC.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"

void UNetGameInstance::Init()
{
	Super::PostInitProperties();

	if ( auto subsys = IOnlineSubsystem::Get() ) {
		sessionInterface = subsys->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);

		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionsComplete);

		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this , &UNetGameInstance::OnJoinSessionCompleted);

		mySessionName.Append(FString::Printf(TEXT("_%d_%d") , FMath::Rand32() , FDateTime::Now().GetMillisecond()));

		/*
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle ,
		FTimerDelegate::CreateLambda([&]
			{
				CreateMySession(mySessionName , 10);
			}
		) , 2.0f , false);
		*/

		
		//FTimerHandle handle;
		//GetWorld()->GetTimerManager().SetTimer(handle,
		//							FTimerDelegate::CreateLambda([&]
		//							{
		//								FindOtherSession();
		//							}
		//							), 2.0f, false);

	}
}

void UNetGameInstance::CreateMySession(FString roomName , int32 playerCount)
{
	FOnlineSessionSettings sessionSettings;

	// 1. Dedicated Server 접속 여부
	sessionSettings.bIsDedicated = false;

	// 2. 랜선(로컬) 매칭을 할 지, steam 매칭을 할 지 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	sessionSettings.bIsLANMatch = (subsysName == "NULL" );

	// 3. 매칭이 온라인을 통해 노출될지 여부
	// false라면, 초대를 통해서만 입장 가능, SenSessioninviteToFriend() 함수를 통해 친구 초대
	sessionSettings.bShouldAdvertise = true;

	// 4. 온라인 상태(Presence) 정보를 활용할지 여부
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. 게임 진행 중에 참여 허가할지 여부
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bAllowJoinInProgress = true;

	// 6. 세션에 참여할 수 있는 공개 연결의 최대 허용 수
	sessionSettings.NumPublicConnections = playerCount;

	// 7. 커스텀 룸네임 설정
	sessionSettings.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. 호스트 네임 설정
	sessionSettings.Set(FName("HOST_NAME"), StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netID
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	PRINTLOG(TEXT("Create Session Start : %s"), *mySessionName);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);

}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName , bool bWasSuccessful)
{
	PRINTLOG(TEXT("SessionName : %s, bWasSuccessful : %d"), *SessionName.ToString(), bWasSuccessful);

	if ( bWasSuccessful == true )
	{
		//GetWorld()->ServerTravel(TEXT("/Game/Net/Maps/BattleMap_Alpha?listen"));
		GetWorld()->ServerTravel(TEXT("/Game/Alpha_Maps/BattleMap_Alpha?listen"));

	}

}

//=====================================================================
void UNetGameInstance::FindOtherSession()
{
	onSearchState.Broadcast(true);

	//--------------------------------------------------------
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. 세션 검색 조건 설정
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 2. Lan 여부
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 3. 최대 검색 세션 수
	sessionSearch->MaxSearchResults = 10;

	// 4. 세션 검색
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());

}

void UNetGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 찾기 실패 시
	if ( !bWasSuccessful ) {
		onSearchState.Broadcast(false);
		PRINTLOG(TEXT("Session search failed..."));
		return;
	}

	// 세션 검색 결과 배열
	auto results = sessionSearch->SearchResults;
	PRINTLOG(TEXT("Search Result Count : %d"), results.Num());

	// 정보를 가져온다
	for ( int i = 0;i < results.Num(); i++ ) {
		auto sr = results[i];

		if ( sr.IsValid() == false ) {
			continue;
		}

		FString roomName;
		FString hostName;

		FSessionInfo sessionInfo;
		sessionInfo.index = i;

		sr.Session.SessionSettings.Get(FName("ROOM_NAME") , roomName);
		sr.Session.SessionSettings.Get(FName("HOST_NAME") , hostName);

		sessionInfo.roomName = StringBase64Decode(roomName);
		sessionInfo.hostName = StringBase64Decode(hostName);

		// 입장 가능한 플레이어 수
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;

		// 현재 입장 가능한 플레이어 수
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;

		sessionInfo.playerCount = FString::Printf(TEXT("(%d/%d)") , currentPlayerCount , maxPlayerCount);


		// 핑 정보(스팀 기준 9999)
		int32 pingSpeed = sr.PingInMs;

		PRINTLOG(TEXT("%s"), *sessionInfo.ToString());
		//PRINTLOG(TEXT("%s : %s(%s) - (%d/%d), %dms") , *roomName , *hostName , *userName , currentPlayerCount , maxPlayerCount , pingSpeed);


		//=========================================================
		onSearchCompleted.Broadcast(sessionInfo);

		onSearchState.Broadcast(false);
	}



}

//=========================================================
FString UNetGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)( utf8String.c_str() ) , utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UNetGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str , arrayData);
	std::string utf8String((char*)( arrayData.GetData() ) , arrayData.Num());
	return UTF8_TO_TCHAR(utf8String.c_str());
}
//=========================================================

void UNetGameInstance::JoinSelectedSession(int32 index)
{
	auto sr = sessionSearch->SearchResults;

	// 이건 현재 언리얼 버그
	sr[index].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	sr[index].Session.SessionSettings.bUsesPresence = true;

	sessionInterface->JoinSession(0 , FName(mySessionName) , sr[index]);
}

void UNetGameInstance::OnJoinSessionCompleted(FName sessionName , EOnJoinSessionCompleteResult::Type result)
{
	if ( result == EOnJoinSessionCompleteResult::Success )
	{
		auto pc = GetWorld()->GetFirstPlayerController();

		FString url;
		sessionInterface->GetResolvedConnectString(sessionName , url);

		PRINTLOG(TEXT("Join URL : %s") , *url);

		if ( url.IsEmpty() == false )
		{
			pc->ClientTravel(url , ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		PRINTLOG(TEXT("Join Session failed : %d") , result);
	}

}

void UNetGameInstance::ExitRoom()
{
	ServerRPC_ExitRoom();
}

void UNetGameInstance::ServerRPC_ExitRoom_Implementation()
{
	MulticastRPC_ExitRoom();
}



void UNetGameInstance::MulticastRPC_ExitRoom_Implementation()
{
	if ( sessionInterface )
	{
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this , &UNetGameInstance::OnMyExitRoomComplete);

		sessionInterface->DestroySession(FName(*mySessionName));
	}
}




void UNetGameInstance::OnMyExitRoomComplete(FName sessionName , bool bWasSuccessful)
{
	UE_LOG(LogTemp , Warning , TEXT("DestroySession Complete: %s, Success: %d") , *sessionName.ToString() , bWasSuccessful);

	if ( bWasSuccessful )
	{
		auto pc = GetWorld()->GetFirstPlayerController();
		FString url = TEXT("/Game/Alpha_Maps/LobbyMap_Alpha");
		if ( pc )
		{
			pc->ClientTravel(url , TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("Failed to destroy session"));
	}
}

bool UNetGameInstance::IsInRoom()
{
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	return sessionInterface->IsPlayerInSession(FName(*mySessionName), *netID);
}

//=========================================================


