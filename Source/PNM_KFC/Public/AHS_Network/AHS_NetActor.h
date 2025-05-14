// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AHS_NetActor.generated.h"

UCLASS()
class PNM_KFC_API AAHS_NetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAHS_NetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//-------------------------------------------------------------
//0416(수) - 프로토 직후
// 테스트용 겸 복습용 액터입니다.
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshComp;

	void PrintNetLog();

	//---------------------------------------
	void FindOwner();

	UPROPERTY(EditAnywhere)
	float searchDistance = 200.0f;

//---------------------------------------
// 속성 리플리케이션
	// 회전 값 동기화 변수
	// 1번 방식
	//UPROPERTY(Replicated)
	//float RotYaw = 0.0f;

	// 2번 방식
	UPROPERTY(ReplicatedUsing=OnRep_RotYaw)
	float RotYaw = 0.0f;

	UFUNCTION()
	void OnRep_RotYaw();

	float currentTime = 0.f;
	float lastTime = 0.f;
//---------------------------------------
	UPROPERTY()
	class UMaterialInstanceDynamic* Mat;

	// 재질에 동기화될 색상
	UPROPERTY(ReplicatedUsing=OnRep_ChangeMatColor)
	FLinearColor MatColor;

	UFUNCTION()
	void OnRep_ChangeMatColor();


//---------------------------------------
//ServerRPC : 서버에서만 실행되는 것들...
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeColor(const FLinearColor newColor);

// ClinetRPC --> 서버에서 호출해야, 클라이언트에도 반영되는 것들
	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeColor(const FLinearColor newColor);

// Net Multicast
	UFUNCTION(Client, Unreliable)
	void MulticastRPC_ChangeColor(const FLinearColor newColor);

	//---------------------------------------




};
