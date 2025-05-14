// Fill out your copyright notice in the Description page of Project Settings.

#include "AHS_Interactables/AC_InteractableComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "AHS_UI/InteractableUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/Image.h"

// Sets default values for this component's properties
UAC_InteractableComponent::UAC_InteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// UI 위젯 블루프린트 클래스를 불러오기
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/AHS/UIS/InteractableUI/WBP_InteractableUI.WBP_InteractableUI_C"));
	if ( WidgetClassFinder.Succeeded() )
	{
		mainUIWidget = WidgetClassFinder.Class;
	}
}

// Called when the game starts
void UAC_InteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 설정
	LocalPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this , 0));

	// 부모 클래스 찾기
	OwnerActor = GetOwner();

	//-------------------------------------------------------
	// 위젯 컴포넌트 생성 및 초기화
	if ( mainUIWidget )
	{
		
		InteractableWidgetComponent = NewObject<UWidgetComponent>(OwnerActor);
		if ( InteractableWidgetComponent )
		{
			InteractableWidgetComponent->RegisterComponent();
			InteractableWidgetComponent->AttachToComponent(OwnerActor->GetRootComponent() , FAttachmentTransformRules::KeepRelativeTransform);
			InteractableWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
			InteractableWidgetComponent->SetDrawAtDesiredSize(true);
			InteractableWidgetComponent->SetWidgetClass(mainUIWidget);
			InteractableWidgetComponent->SetTwoSided(true);
			InteractableWidgetComponent->SetRelativeLocation(FVector(0.f , 0.f , 0.f)); // 원하는 위치로 조정
			InteractableWidgetComponent->SetHiddenInGame(true); // 초기에는 숨김

			// 3D 오브젝트 위에 그려지도록.
			InteractableWidgetComponent->SetTranslucentSortPriority(20);


			//----------------------------------------------------------------------
			InteractableWidgetComponent->InitWidget(); // 위젯 생성 보장
			if ( UInteractableUI* InteractUI = Cast<UInteractableUI>(InteractableWidgetComponent->GetUserWidgetObject()) )
			{
				if ( InteractUI->Img_Interactable )
				{
					// 적이면 빨간색, 아니면 초록색 tint 적용
					FLinearColor TintColor = bIsEnemy ? FLinearColor::Red : FLinearColor::Green;
					InteractUI->Img_Interactable->SetColorAndOpacity(TintColor);
				}
			}

		}
	}
}

// Called every frame
void UAC_InteractableComponent::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);

	// 예외 처리
	if ( !InteractableWidgetComponent || !LocalPlayer )
	{
		return;
	}
	//--------------------------------------------------------------------
	// 플레이어가 행동 중이라면 UI 끄기
	if ( LocalPlayer->isAttacking || LocalPlayer->isInteracting ) {
		HideUI();
	}

	//--------------------------------------------------------------------
	const bool bShouldShowUI = ( LocalPlayer->CurrentTarget == OwnerActor );

	// UI 표시 여부
	if ( bShouldShowUI && !bIsUIVisible )
	{
		ShowUI();
		bIsUIVisible = true;
	}
	else if ( !bShouldShowUI && bIsUIVisible )
	{
		HideUI();
		bIsUIVisible = false;
	}

	// UI가 표시 중일 때만 위치/회전 갱신
	if ( bIsUIVisible )
	{
		FVector CameraLocation = LocalPlayer->PlayerCameraComp->GetComponentLocation();
		FVector WidgetLocation = InteractableWidgetComponent->GetComponentLocation();
		FVector Direction = ( CameraLocation - WidgetLocation ).GetSafeNormal();

		FRotator CurrentRotation = InteractableWidgetComponent->GetComponentRotation();
		FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation , TargetRotation , DeltaTime , 10.f);
		InteractableWidgetComponent->SetWorldRotation(SmoothRotation);

		/*
		FVector TargetLocation = OwnerActor->GetActorLocation() + FVector(0.f , 0.f , 100.f);
		FVector CurrentLocation = InteractableWidgetComponent->GetComponentLocation();
		FVector SmoothedLocation = FMath::VInterpTo(CurrentLocation , TargetLocation , DeltaTime , 10.f);
		InteractableWidgetComponent->SetWorldLocation(SmoothedLocation);
		*/
	}
}

void UAC_InteractableComponent::SetTargetable(bool bTargetable)
{
	bCanTarget = bTargetable;
}

void UAC_InteractableComponent::ShowUI()
{
	if ( InteractableWidgetComponent )
	{
		InteractableWidgetComponent->SetHiddenInGame(false);

	}
}

void UAC_InteractableComponent::HideUI()
{

	InteractableWidgetComponent->SetHiddenInGame(true);
}
