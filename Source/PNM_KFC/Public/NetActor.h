// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetActor.generated.h"

UCLASS()
class PNM_KFC_API ANetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FindOwner();

	UPROPERTY(EditAnywhere)
	float SearchDistance = 200.0f;
	
	UPROPERTY(Replicated)
	float RotYaw = 0.0f;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const;
};
