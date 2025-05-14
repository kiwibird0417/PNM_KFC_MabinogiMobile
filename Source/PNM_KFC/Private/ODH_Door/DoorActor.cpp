// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_Door/DoorActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp=CreateDefaultSubobject<UBoxComponent>(L"Collision");
	SetRootComponent(BoxComp);


}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitLocation = GetActorLocation();
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if( bIsClera)
	{ 
		CurTime += DeltaTime;
		float t = FMath::Clamp(CurTime / AnimationDuration, 0.0f, 1.0f);

		// 이징 함수 적용
		float EasedValue = EaseInOutBounce(t);

		// 문 위치 계산
		FVector NewLocation = InitLocation + FVector(0.0f , 0.0f , MoveDistance * EasedValue);
		SetActorLocation(NewLocation);

		// 애니메이션 종료
		if ( t >= 1.0f )
		{
			bIsAnimating = false;
		}
	}
}

void ADoorActor::OpenDoor()
{
	if ( !bIsClera )
	{
		bIsClera = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
	}
}

float ADoorActor::EaseInOutBounce(float t)
{
	if ( t < 0.5f )
	{
		return 0.5f * ( 1.0f - EaseOutBounce(1.0f - 2.0f * t) );
	}
	else
	{
		return 0.5f * ( 1.0f + EaseOutBounce(2.0f * t - 1.0f) );
	}
}

float ADoorActor::EaseOutBounce(float t)
{
	if ( t < 1.0f / 2.75f )
	{
		return 7.5625f * t * t;
	}
	else if ( t < 2.0f / 2.75f )
	{
		t -= 1.5f / 2.75f;
		return 7.5625f * t * t + 0.75f;
	}
	else if ( t < 2.5f / 2.75f )
	{
		t -= 2.25f / 2.75f;
		return 7.5625f * t * t + 0.9375f;
	}
	else
	{
		t -= 2.625f / 2.75f;
		return 7.5625f * t * t + 0.984375f;
	}
}

