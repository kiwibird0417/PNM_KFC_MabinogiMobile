// Fill out your copyright notice in the Description page of Project Settings.


#include "NetActor.h"
#include "AHS_Player/PlayerCharacter.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANetActor::ANetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void ANetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetActor , RotYaw);
}

// Called when the game starts or when spawned
void ANetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindOwner();
}

void ANetActor::FindOwner()
{
	if ( HasAuthority() )
	{
		APlayerCharacter* newOwner = nullptr;
		float minDist = SearchDistance;

		for ( TActorIterator<APlayerCharacter> it(GetWorld()); it; ++it )
		{
			APlayerCharacter* otherPlayer = *it;
			float dist = GetDistanceTo(otherPlayer);
			if ( dist < minDist )
			{
				minDist = dist;
				newOwner = otherPlayer;
			}
		}

		if ( GetOwner() != newOwner )
		{
			SetOwner(newOwner);
		}
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), SearchDistance, 30, FColor::Yellow, false, 0 ,0 ,1);
}