// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_Boss/Tornado.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "AHS_Player/PlayerCharacter.h"

// Sets default values
ATornado::ATornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(L"Collision");
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(113);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATornado::HitPlayerStart);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ATornado::HitPlayerEnd);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(L"Particle");
	Particle->SetupAttachment(SphereComp);

	ConstructorHelpers::FObjectFinder<UParticleSystem> temp_particle(TEXT("/Script/Engine.ParticleSystem'/Game/ODH/Asset/AttackEffect/Particles/P_Tornado.P_Tornado'"));
	if ( temp_particle.Succeeded() )
	{
		Particle->SetTemplate(temp_particle.Object);
	}
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();
	
	/*HitDelayTime = 0.0f;*/
}

// Called every frame
void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurTime += DeltaTime;

	if ( CurTime >= DestroyTime )
	{
		Destroy();
	}

	CheckOverlap();

// 	if ( HitDelayTime >= 0.0f )
// 	{
// 		HitDelayTime -= DeltaTime;
// 	}
}

void ATornado::CheckOverlap()
{
	if ( OverlapPlayers.Num() > 0 )
	{
		for ( APlayerCharacter* player : OverlapPlayers )
		{
			player->ApplyTornadoDamagetoPlayer(8);
		}
	}
}

void ATornado::HitPlayerStart(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if(player )
	{
		OverlapPlayers.AddUnique(player);
	}
}

void ATornado::HitPlayerEnd(UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if ( player )
	{
		OverlapPlayers.Remove(player);
	}
}

