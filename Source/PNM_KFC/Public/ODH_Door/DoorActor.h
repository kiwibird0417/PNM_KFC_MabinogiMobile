// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class PNM_KFC_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//웨이브가 처리됐을 때 문이 열리는 함수
	void OpenDoor();

	bool bIsClera = false;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* OpenSound;

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	// 기본값 설정
	float MoveDistance = 600.0f; // 100 유닛 위로 이동
	float AnimationDuration = 2.0f; // 2초 동안 애니메이션
	bool bIsAnimating = false;
	float CurTime = 0.0f;

	//문 열릴때 속도
	float Speed = 400.0f;

	float EaseInOutBounce(float t);
	float EaseOutBounce(float t);

	FVector InitLocation;
};
