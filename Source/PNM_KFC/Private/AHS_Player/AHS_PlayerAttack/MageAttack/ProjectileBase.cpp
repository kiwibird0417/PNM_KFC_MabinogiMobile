// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Player/AHS_PlayerAttack/MageAttack/ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AProjectileBase::InitProjectile(FVector StartLocation , FVector TargetLocation)
{

}

void AProjectileBase::Fire()
{

}

void AProjectileBase::OnImpact(AActor* HitActor)
{

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

