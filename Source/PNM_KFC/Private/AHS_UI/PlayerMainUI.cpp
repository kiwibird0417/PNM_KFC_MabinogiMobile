// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/PlayerMainUI.h"
#include "Components/Button.h"
#include "AHS_Player/PlayerCharacter.h"
#include "Components/Image.h"

void UPlayerMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 1. 플레이어 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());


	// 1-1. 상호작용 버튼 연결
	if ( Button_Interaction )
	{
		Button_Interaction->OnClicked.AddDynamic(this , &UPlayerMainUI::OnInterActionButtonClicked);
	}

	//----------------------------------------------------------
	// 1-2. 플레이어 인벤토리 버튼 연결
	if ( Button_Inventory )
	{
		Button_Inventory->OnClicked.AddDynamic(this , &UPlayerMainUI::OnClickInventoryButton);
	}


}


//----------------------------------------------------------------------------
// 자동 버튼 활성화/비활성화
void UPlayerMainUI::OnInterActionButtonClicked()
{
	OwnerCharacter->SetAuto();

	bool bIsAuto = OwnerCharacter->bIsAuto;

	ChangeInterActionButtonImage(bIsAuto);

}


// 자동 버튼 이미지 변경
void UPlayerMainUI::ChangeInterActionButtonImage(bool bIsAuto)
{
	if ( !OwnerCharacter )
	{
		OwnerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	}

	if ( !OwnerCharacter || !OwnerCharacter->IsLocallyControlled() )
	{
		UE_LOG(LogTemp , Warning , TEXT("UI OwnerCharacter is null or not locally controlled"));
		return;
	}

	FButtonStyle ButtonStyle = Button_Interaction->WidgetStyle;

	if ( bIsAuto )
	{
		// 자동 모드
		if ( Texture_Auto )
		{
			FSlateBrush AutoBrush;
			AutoBrush.SetResourceObject(Texture_Auto);
			ButtonStyle.Normal = AutoBrush;
			ButtonStyle.Hovered = AutoBrush;
			ButtonStyle.Pressed = AutoBrush;
		}
	}
	else
	{
		if ( OwnerCharacter->canAttack )
		{
			if ( Texture_Attack )
			{
				FSlateBrush AutoBrush;
				AutoBrush.SetResourceObject(Texture_Attack);
				ButtonStyle.Normal = AutoBrush;
				ButtonStyle.Hovered = AutoBrush;
				ButtonStyle.Pressed = AutoBrush;
			}
		}
		else if ( OwnerCharacter->canInteract )
		{
			if ( Texture_Normal_Interaction )
			{
				FSlateBrush AutoBrush;
				AutoBrush.SetResourceObject(Texture_Normal_Interaction);
				ButtonStyle.Normal = AutoBrush;
				ButtonStyle.Hovered = AutoBrush;
				ButtonStyle.Pressed = AutoBrush;
			}
		}
		else
		{
			if ( Texture_Manual )
			{
				FSlateBrush ManualBrush;
				ManualBrush.SetResourceObject(Texture_Manual);
				ButtonStyle.Normal = ManualBrush;
				ButtonStyle.Hovered = ManualBrush;
				ButtonStyle.Pressed = ManualBrush;
			}
		}
	}

	Button_Interaction->SetStyle(ButtonStyle);
}


//----------------------------------------------------------------
// 인벤토리 버튼 누르기
void UPlayerMainUI::OnClickInventoryButton()
{

}


//----------------------------------------------------------------
// UPlayerMainUI 안에
void UPlayerMainUI::SetOwnerCharacter(APlayerCharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
}



