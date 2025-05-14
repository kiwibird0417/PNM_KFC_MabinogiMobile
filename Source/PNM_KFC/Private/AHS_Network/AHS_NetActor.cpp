// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Network/AHS_NetActor.h"
#include "PNM_KFC.h"
#include "Components/StaticMeshComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AAHS_NetActor::AAHS_NetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	meshComp->SetRelativeScale3D(FVector(0.5f));

	// 네트워크와 동기화
	bReplicates = true;

	// 대역폭 조정, 기본값 = 100
	NetUpdateFrequency = 100.0f;

}

// Called when the game starts or when spawned
void AAHS_NetActor::BeginPlay()
{
	Super::BeginPlay();
	
	// meshComp의 0번째 머터리얼을 기반으로 동적 머티리얼 인스턴스 생성
	Mat = meshComp->CreateAndSetMaterialInstanceDynamic(0);

	// 서버일 때
	if ( HasAuthority() ) {
		// 1초 주기로 MatColor르ㄹ 랜덤하게 변경
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle , FTimerDelegate::CreateLambda([&] {
			MatColor = FLinearColor(FMath::RandRange(0.f, .3f), FMath::RandRange(0.f , .3f), FMath::RandRange(0.f , .3f));

			OnRep_ChangeMatColor();
			
		}),
		1.f,
		true
		);
	}

}

// Called every frame
void AAHS_NetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindOwner();
	PrintNetLog();

	//-------------------------------------------------
	// Server일 경우
	if ( HasAuthority() ) {
		// 서버 영역
		AddActorLocalRotation(FRotator(0.0f, 50.0f * DeltaTime, 0.0f));
		RotYaw = GetActorRotation().Yaw;
	}
	else {
		// 클라이언트 자체 보간
		// 경과 시간 증가
		currentTime += DeltaTime;

		// 0으로 나눠지지 않도록 lastTime 값 체크
		if ( lastTime < KINDA_SMALL_NUMBER ) {
			return;
		}

		// 이전 경과시간과 현재 경과시간의 비율 계산
		float lerpRatio = currentTime / lastTime;
		
		// 이전 경과 시간만큼 회전할 것으로 새로운 회전값 계산
		float newYaw = RotYaw + 50 * lastTime;

		// 예측되는 값으로 진행된 시간만큼 보간처리
		float lerpYaw = FMath::Lerp(RotYaw, newYaw, lerpRatio);

		// 최종 적용
		FRotator CurRot = GetActorRotation();
		CurRot.Yaw = lerpYaw;
		SetActorRotation( CurRot);
		
		// 클라이언트 영역
		//FRotator NewRot = GetActorRotation();
		//NewRot.Yaw = RotYaw;
		//SetActorRotation(NewRot);
	}

	//-------------------------------------------------


}

void AAHS_NetActor::PrintNetLog()
{
	const FString conStr = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");
	const FString ownerName = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	const FString logStr = FString::Printf(TEXT("Connection : %s \nOwner Name : %s\nLocalRole : %s\nRemote Role : %s") , *conStr , *ownerName , *LOCAL_ROLE , *REMOTE_ROLE);

	DrawDebugString(GetWorld() , GetActorLocation() + FVector::UpVector * 100.0f , logStr , nullptr , FColor::Red , 0 , true);
}

void AAHS_NetActor::FindOwner()
{
	if ( HasAuthority() ) {
		AActor* newOwner = nullptr;
		float minDist = searchDistance;

		for ( TActorIterator<APlayerCharacter> it(GetWorld()); it; ++it ) {
			AActor* otherActor = *it;
			float dist = GetDistanceTo(otherActor);
			if ( dist < minDist ) {
				minDist = dist;
				newOwner = otherActor;
			}
		}

		// Owner 설정
		if ( GetOwner() != newOwner ) {
			SetOwner(newOwner);
		}

		DrawDebugSphere(GetWorld(), GetActorLocation(), searchDistance, 30, FColor::Yellow, false, 0, 0, 1);
	}
}

void AAHS_NetActor::OnRep_RotYaw()
{
	// 클라이언트 영역
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw = RotYaw;
	SetActorRotation(NewRot);

	// 업데이트된 경과시간 저장
	lastTime = currentTime;

	// 경과시간 초기화
	currentTime = 0.f;
}

void AAHS_NetActor::OnRep_ChangeMatColor()
{
	if ( Mat ) {
		Mat->SetVectorParameterValue(TEXT("FloorColor"), MatColor);
	}
}


// Server RPC 방식
// 서버에서 ServerRPC르ㄹ 호출하기 때문에 서버에서만 색이 변경되고, 클라에서는 변화가 없다.
void AAHS_NetActor::ServerRPC_ChangeColor_Implementation(const FLinearColor newColor)
{
	//if ( Mat ) {
	//	Mat->SetVectorParameterValue(TEXT("FloorColor"), newColor);
	//}
	
	// 클라에서도 변화를 준다.
	//ClientRPC_ChangeColor(newColor);

	// 둘 다 변화 주기(Owner가 없어도 동기화 가능)
	MulticastRPC_ChangeColor(newColor);
}

void AAHS_NetActor::ClientRPC_ChangeColor_Implementation(const FLinearColor newColor)
{
	if ( !Mat) return;
	Mat->SetVectorParameterValue(TEXT("FloorColor") , newColor);

}

void AAHS_NetActor::MulticastRPC_ChangeColor_Implementation(const FLinearColor newColor)
{
	if ( !Mat ) return;
	Mat->SetVectorParameterValue(TEXT("FloorColor") , newColor);
}

//-------------------------------------------------------------------------
// 속성 리플리케이션
void AAHS_NetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAHS_NetActor, RotYaw);
	DOREPLIFETIME(AAHS_NetActor, MatColor);
}
