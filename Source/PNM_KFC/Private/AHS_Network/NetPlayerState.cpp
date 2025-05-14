// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Network/NetPlayerState.h"
#include "AHS_Network/NetGameInstance.h"
#include "AHS_Network/NetPlayerController.h"

// NetPlayerState.cpp

//void ANetPlayerState::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// 로컬 클라이언트에서만 실행하도록 수정
//	if ( GetPlayerController() && GetPlayerController()->IsLocalController() ) {
//		auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
//		if ( gi ) {
//			// 서버에 이름을 전송
//			ServerRPC_SetUserName(gi->mySessionName);
//		}
//	}
//}
//
//void ANetPlayerState::ServerRPC_SetUserName_Implementation(const FString& name)
//{
//	// 서버에서 이름을 설정하고, 모든 클라이언트에게 전달
//	SetPlayerName(name);
//	MulticastRPC_SetUserName(name);
//}
//
//// 추가된 유효성 검사 함수
//bool ANetPlayerState::ServerRPC_SetUserName_Validate(const FString& Name)
//{
//	// 예시: 이름이 비어 있지 않은지 확인
//	return !Name.IsEmpty();
//}
//
//void ANetPlayerState::MulticastRPC_SetUserName_Implementation(const FString& name)
//{
//	// 모든 클라이언트에서 이름을 설정
//	SetPlayerName(name);
//}


void ANetPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// 로컬 클라이언트에서만 실행하도록 수정
	if ( GetPlayerController() && GetPlayerController()->IsLocalController() ) {
		auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
		if ( gi ) {
			// 서버에 이름을 전송
			ServerRPC_SetUserName(gi->mySessionName);
		}
	}
}

void ANetPlayerState::ServerRPC_SetUserName_Implementation(const FString& name)
{
	// 서버에서 이름을 설정
	SetPlayerName(name);

	// 이름 설정이 완료된 후 클라이언트에서 UI를 업데이트하도록 함
	ANetPlayerController* playerController = Cast<ANetPlayerController>(GetPlayerController());
	if ( playerController )
	{
		// 로컬 클라이언트만 UI 업데이트
		playerController->ClientUpdatePlayerName(name);
	}
}

bool ANetPlayerState::ServerRPC_SetUserName_Validate(const FString& Name)
{
	// 이름이 비어 있지 않은지 확인
	return !Name.IsEmpty();
}

