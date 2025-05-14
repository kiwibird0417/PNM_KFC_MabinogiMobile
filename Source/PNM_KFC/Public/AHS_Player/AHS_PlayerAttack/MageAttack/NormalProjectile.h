// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHS_Player/AHS_PlayerAttack/MageAttack/ProjectileBase.h"
#include "NormalProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PNM_KFC_API ANormalProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	ANormalProjectile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
    virtual void InitProjectile(FVector StartLocation, FVector TargetLocation);
    //virtual void Fire();
    //virtual void OnImpact(AActor* HitActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
    class USphereComponent* Collision;

	// 플레이어 캐릭터 참조
	class APlayerCharacter* Player;

	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
								  UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
								  const FHitResult& SweepResult);


	
};
