// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Network/NetPlayerController.h"
#include "GameFramework/Actor.h"
#include "PNM_KFCGameMode.h"
#include "AHS_UI/PlayerMainUI.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpectatorPawn.h"
#include "AHS_UI/Player_Multi/Net_PlayerUI.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "ODH_UI/ClearWidget.h"




void ANetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if ( HasAuthority() ) {
		gm = Cast<APNM_KFCGameMode>(GetWorld()->GetAuthGameMode());
	}
}

void ANetPlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	auto player = GetPawn();
	UnPossess();
	player->Destroy();

	gm->RestartPlayer(this);
}

void ANetPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	auto player = GetPawn();

	if ( player ) {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetActorTransform(), params);

		// 빙의
		Possess(spectator);

		//이전 플레이어 제거
		player->Destroy();

		// 5초 후에 리스폰 시키기
		FTimerHandle handle;

		GetWorldTimerManager().SetTimer(handle, this, &ANetPlayerController::ServerRPC_RespawnPlayer_Implementation, 5.0, false);
	}
}

void ANetPlayerController::ClientUpdatePlayerName_Implementation(const FString& name)
{
}

void ANetPlayerController::ClientRPC_ShowClearUI_Implementation()
{
	if ( ClearUI )
	{
		UUserWidget* clearUIWidget =CreateWidget(this , ClearUI);

		if ( clearUIWidget )
		{
			clearUIWidget->AddToViewport();

			Cast<UClearWidget>(clearUIWidget)->PlayUIAnim();
		}

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}







