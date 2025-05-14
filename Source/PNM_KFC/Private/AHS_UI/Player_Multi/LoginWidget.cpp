// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/Player_Multi/LoginWidget.h"
#include "AHS_Network/NetGameInstance.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "AHS_UI/Player_Multi/SessionSlotWidget.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	GI->onSearchCompleted.AddDynamic(this , &ULoginWidget::AddSlotWidget);
	GI->onSearchState.AddDynamic(this , &ULoginWidget::OnChangeButtonEnable);
	
	btn_createRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateRoom);
	slider_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);

	btn_createSession->OnClicked.AddDynamic(this , &ULoginWidget::SwitchCreatePanel);
	btn_findSession->OnClicked.AddDynamic(this , &ULoginWidget::SwitchFindPanel);

	btn_back->OnClicked.AddDynamic(this , &ULoginWidget::BackToMain);
	btn_back_1->OnClicked.AddDynamic(this , &ULoginWidget::BackToMain);

	

	btn_find->OnClicked.AddDynamic(this , &ULoginWidget::OnClickedFindSession);
}

void ULoginWidget::CreateRoom()
{
	if ( !GI || edit_roomName->GetText().IsEmpty() ) {
		return;
	}

	GI->CreateMySession(edit_roomName->GetText().ToString(), slider_playerCount->GetValue());

}

void ULoginWidget::OnValueChanged(float Value)
{
	txt_playerCount->SetText(FText::AsNumber(Value));
}

//=========================================================================
void ULoginWidget::SwitchCreatePanel()
{
	if ( edit_userName->GetText().IsEmpty() == false ) {
		GI->mySessionName = edit_userName->GetText().ToString();
	}

	WidgetSwitcher_Lobby->SetActiveWidgetIndex(3);
}

void ULoginWidget::SwitchFindPanel()
{
	if ( edit_userName->GetText().IsEmpty() == false ) {
		GI->mySessionName = edit_userName->GetText().ToString();
	}

	WidgetSwitcher_Lobby->SetActiveWidgetIndex(4);
	OnClickedFindSession();
}

void ULoginWidget::BackToMain()
{
	WidgetSwitcher_Lobby->SetActiveWidgetIndex(2);
}

//=========================================================================


void ULoginWidget::AddSlotWidget(const struct FSessionInfo& InSessionInfo)
{
	auto slot = CreateWidget<USessionSlotWidget>(this, SessionInfoWidget);
	slot->Set(InSessionInfo);

	Scroll_RoomList->AddChild(slot);
}

//=========================================================================

void ULoginWidget::OnClickedFindSession()
{
	Scroll_RoomList->ClearChildren();

	if ( GI != nullptr ) {
		GI->FindOtherSession();
	}
}

void ULoginWidget::OnChangeButtonEnable(bool bIsSearching)
{
	btn_find->SetIsEnabled(!bIsSearching);

	if ( bIsSearching == true )
	{
		// 검색중 보이도록 처리
		txt_findingMsg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 검색중 사라지도록 처리
		txt_findingMsg->SetVisibility(ESlateVisibility::Hidden);
	}
}

//=========================================================================
