// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHS_Player/AHS_PlayerAttack/MageAttack/ProjectileBase.h"


#include "ElectroProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API AElectroProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AElectroProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

//=========================================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;


	UPROPERTY(VisibleAnywhere)
    class USphereComponent* CollisionComp;

	//=========================================================================
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
				   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
				   bool bFromSweep, const FHitResult& SweepResult);

	//=========================================================================
	/** BFS 방식 전기 감전 */
	void ChainLightning_BFS(AActor* StartEnemy, float MaxDistance, int MaxChains);

	/** 거리순 정렬 방식 전기 감전 */
	void ChainLightning_SortByDistance(AActor* StartEnemy, float MaxDistance, int MaxChains);

	//=========================================================================
	class APlayerCharacter* Player;

	class UProjectileMovementComponent* ProjectileMovement;
	
};
