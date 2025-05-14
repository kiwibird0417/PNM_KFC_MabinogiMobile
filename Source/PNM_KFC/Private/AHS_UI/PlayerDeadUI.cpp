// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerDeadUI.h"
#include "Components/Button.h"
#include "AHS_Network/NetPlayerController.h"

void UPlayerDeadUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Resurrect->OnClicked.AddDynamic(this, &UPlayerDeadUI::OnRetry);
	Button_LeaveRoom->OnClicked.AddDynamic(this, &UPlayerDeadUI::OnExit);

}

void UPlayerDeadUI::OnRetry()
{
	this->SetVisibility(ESlateVisibility::Hidden);

	auto pc = Cast<ANetPlayerController>(GetOwningPlayer());
	if ( pc ) {
		pc->SetShowMouseCursor(false);
		
		pc->ServerRPC_RespawnPlayer();

		//pc->ServerRPC_ChangeToSpectator();
	}
}


void UPlayerDeadUI::OnExit()
{

}
