// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_UI/InteractableUI.h"

void UInteractableUI::PlayFadeIn()
{
	if ( FadeAnim )
	{
		PlayAnimation(FadeAnim , 0.f , 1 , EUMGSequencePlayMode::Forward);
	}
}

void UInteractableUI::PlayFadeOut()
{
	if ( FadeAnim )
	{
		PlayAnimation(FadeAnim , 0.f , 1 , EUMGSequencePlayMode::Reverse);
	}
}
