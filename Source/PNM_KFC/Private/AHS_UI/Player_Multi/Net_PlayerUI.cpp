// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/Player_Multi/Net_PlayerUI.h"
#include "Components/TextBlock.h"
#include "AHS_Network/NetPlayerController.h"
#include "AHS_Network/NetPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "AHS_Network/NetGameInstance.h"

//void UNet_PlayerUI::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	//// 플레이어의 PlayerState를 가져옴
//	//auto* pc = GetWorld()->GetFirstPlayerController();
//	//if ( pc )
//	//{
//	//	ANetPlayerState* PlayerState = Cast<ANetPlayerState>(pc->PlayerState);
//	//	if ( PlayerState && txt_nickName )
//	//	{
//	//		// 자기 자신의 이름을 표시 (서버에서 받은 이름)
//	//		txt_nickName->SetText(FText::FromString(PlayerState->GetPlayerName()));
//	//	}
//	//}
//
//	auto* netPlayerState = GetOwningPlayerState<ANetPlayerState>();
//	if ( netPlayerState )
//	{
//		txt_nickName->SetText(FText::FromString(netPlayerState->GetPlayerName()));
//	}
//}

void UNet_PlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 자신의 이름을 UI에 설정
	//auto* netPlayerState = GetOwningPlayerState<ANetPlayerState>();
	//if ( netPlayerState )
	//{
	//	// 자기 자신의 이름을 UI에 설정
	//	txt_nickName->SetText(FText::FromString(netPlayerState->GetPlayerName()));
	//}
}

void UNet_PlayerUI::SetPlayerName(const FString& name)
{
	if ( txt_nickName )
	{
		txt_nickName->SetText(FText::FromString(name));
	}
}

