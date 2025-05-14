// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_UI/ClearWidget.h"
#include "AHS_Network/NetGameInstance.h"
#include "Components/Button.h"

void UClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_exit->OnClicked.AddDynamic(this, &UClearWidget:: OnExit);
}

void UClearWidget::OnExit()
{
	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if(gi)
	{
		gi->ExitRoom();
	}
}

void UClearWidget::PlayUIAnim()
{
	PlayAnimation(ClearAnimation);
}
