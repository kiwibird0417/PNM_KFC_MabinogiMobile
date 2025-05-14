// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH_Enemy/ZODH_NetActor.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "AHS_Player/PlayerCharacter.h"

// Sets default values
AZODH_NetActor::AZODH_NetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp= CreateDefaultSubobject<UStaticMeshComponent>(L"Mesh");
	SetRootComponent(MeshComp);

	bReplicates = true;
}

void AZODH_NetActor::OnRep_RotYaw()
{
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw = RotYaw;
	SetActorRotation(NewRot);

	lastTime = CurTime;

	CurTime = 0.0f;
}

void AZODH_NetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZODH_NetActor , RotYaw);
	DOREPLIFETIME(AZODH_NetActor , MatColor);
}

void AZODH_NetActor::OnRep_ChangeMatColor()
{
	if ( Mat )
	{
		Mat->SetVectorParameterValue(TEXT("FloorColor"),MatColor);
	}
}

void AZODH_NetActor::ServerRPC_ChangeColor_Implementation(const FLinearColor InNewColor)
{
// 	if(!Mat )return;
// 	Mat->SetVectorParameterValue(L"FloorColor",InNewColor);
// 	ClientRPC_ChangeColor(InNewColor);
	MultiCastRPC_ChangeColor(InNewColor);
}

void AZODH_NetActor::ClientRPC_ChangeColor_Implementation(const FLinearColor newColor)
{
	if(!Mat ) return;
	Mat->SetVectorParameterValue(L"FloorColor",newColor);
}

void AZODH_NetActor::MultiCastRPC_ChangeColor_Implementation(const FLinearColor newColor)
{
	if ( !Mat ) return;
	Mat->SetVectorParameterValue(L"FloorColor" , newColor);
}

// Called when the game starts or when spawned
void AZODH_NetActor::BeginPlay()
{
	Super::BeginPlay();

	Mat = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

	if ( HasAuthority() )
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle , FTimerDelegate::CreateLambda([&]
			{
				MatColor = FLinearColor(FMath::RandRange(0.0f , 0.3f) , FMath::RandRange(0.0f , 0.3f) ,
				   FMath::RandRange(0.0f , 0.3f) , 1.0f);
				//OnRep_ChangeMatColor();
				ServerRPC_ChangeColor_Implementation(MatColor);
			}) , 1.0f , true);

	}
}

// Called every frame
void AZODH_NetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindOwner();

	if ( HasAuthority() )
	{
		AddActorLocalRotation(FRotator(0,50.0f * DeltaTime, 0));
		RotYaw = GetActorRotation().Yaw;
	}

	else
	{
		CurTime+=DeltaTime;
		if ( lastTime < KINDA_SMALL_NUMBER )
		{
			return;
		}

		float lerpRatio = CurTime / lastTime;

		float newYaw = RotYaw + 50 * lastTime;

		float lerpYaw = FMath::Lerp(RotYaw, newYaw, lerpRatio);

		FRotator CurRot = GetActorRotation();
		CurRot.Yaw = lerpYaw;
		SetActorRotation(CurRot);
	}
}

void AZODH_NetActor::FindOwner()
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

	DrawDebugSphere(GetWorld() , GetActorLocation() , SearchDistance , 30 , FColor::Yellow , false , 0 , 0 , 1);
}