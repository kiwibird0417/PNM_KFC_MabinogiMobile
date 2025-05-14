// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AHS_Player/PlayerCharacter.h"
#include "Tornado.generated.h"

UCLASS()
class PNM_KFC_API ATornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATornado();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float CurTime = 0.0f;
	float DestroyTime = 4.5f;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* Particle;

// 	bool bOverlapPlayer = false;

	TArray<class APlayerCharacter*> OverlapPlayers;

	void CheckOverlap();

	UFUNCTION()
	void HitPlayerStart(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION()
	void HitPlayerEnd(UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex);
};
