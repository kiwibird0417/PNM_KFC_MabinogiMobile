// Copyright Epic Games, Inc. All Rights Reserved.

#include "PNM_KFCGameMode.h"
#include "PNM_KFCCharacter.h"
#include "UObject/ConstructorHelpers.h"

APNM_KFCGameMode::APNM_KFCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
