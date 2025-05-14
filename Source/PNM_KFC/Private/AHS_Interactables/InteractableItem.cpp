// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Interactables/InteractableItem.h"
#include "AHS_Interactables/AC_InteractableComponent.h"

// Sets default values
AInteractableItem::AInteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 2. 플레이어 입력 컴포넌트 추가
	CanInteractComp = CreateDefaultSubobject<UAC_InteractableComponent>(TEXT("CanInteractComp"));

}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

