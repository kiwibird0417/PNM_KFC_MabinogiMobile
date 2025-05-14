// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerInfoUI.h"
#include "Components/TextBlock.h"
#include "AHS_Network/NetPlayerState.h"
#include "AHS_Network/NetPlayerController.h"
#include "AHS_Network/NetGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "AHS_Player/PlayerCharacter.h"
#include "AHS_UI/Player_Multi/ChatWidget.h"
#include "Components/ScrollBox.h"

void UPlayerInfoUI::NativeConstruct()
{
	Super::NativeConstruct();


	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if ( gi )
	{
		UE_LOG(LogTemp , Error , TEXT("mySessionName: %s") , *gi->mySessionName);

		if ( txt_myName )
		{
			txt_myName->SetText(FText::FromString(gi->mySessionName));
		}
	}

	// 방 종료 버튼 연결
	if ( btn_exit )
	{
		btn_exit->OnClicked.AddDynamic(this , &UPlayerInfoUI::OnExit);
	}

	// 채팅 보내기 버튼 연결
	if ( btn_send ) {
		btn_send->OnClicked.AddDynamic(this, &UPlayerInfoUI::SendMsg);
	}
}

void UPlayerInfoUI::SetPlayerLevelText(int32 currentPlayerLevel)
{
	if ( PlayerLevelText )
	{
		PlayerLevelText->SetText(FText::AsNumber(currentPlayerLevel));
	}
}

void UPlayerInfoUI::SetPlayerGoldText(int32 currentPlayerGold)
{
	if ( PlayerGoldText ) {
		PlayerGoldText->SetText(FText::AsNumber(currentPlayerGold));
	}
}

void UPlayerInfoUI::UpdatePlayerName()
{
	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if ( gi && !gi->mySessionName.IsEmpty() ) {
		txt_myName->SetText(FText::FromString(gi->mySessionName));
	}
}

void UPlayerInfoUI::OnExit()
{
	
	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());

	if ( gi ) {
		gi->ExitRoom();
	}
}

void UPlayerInfoUI::SendMsg()
{
	UE_LOG(LogTemp, Warning, TEXT("Sended the Message"));

	FString msg = edit_input->GetText().ToString();
	edit_input->SetText(FText::GetEmpty());
	if(msg.IsEmpty() ){ return; }

	auto pc = Cast<ANetPlayerController>(GetWorld()->GetFirstPlayerController());
	if ( pc ) {
		auto player = Cast<APlayerCharacter>(pc->GetPawn());
		if ( player ) {
			player->ServerRPC_SendMsg(msg);

			UE_LOG(LogTemp , Warning , TEXT("Succeeded sending the Message"));
		}
	}
}

void UPlayerInfoUI::ReceiveMsg(const FString& msg)
{
	if ( !IsValid(this) )
	{
		UE_LOG(LogTemp, Error, TEXT("Is not valid!"));
		return;
	}


	if ( !chatWidget )
	{
		UE_LOG(LogTemp , Error , TEXT("chatWidget is null!"));
		return;
	}

	auto msgWidget = CreateWidget<UChatWidget>(GetWorld() , chatWidget);
	if ( !msgWidget || !msgWidget->txt_msg )
	{
		UE_LOG(LogTemp , Error , TEXT("msgWidget or txt_msg is null!"));
		return;
	}

	msgWidget->txt_msg->SetText(FText::FromString(msg));

	if ( !scroll_msgList )
	{
		UE_LOG(LogTemp , Error , TEXT("scroll_msgList is null!"));
		return;
	}

	scroll_msgList->AddChild(msgWidget);
	scroll_msgList->ScrollToEnd();
}

