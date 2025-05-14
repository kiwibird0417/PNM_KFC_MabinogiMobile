// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestEnemy.generated.h"

UCLASS()
class PNM_KFC_API ATestEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ATestEnemy();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//자신이 어느 데이터를 가져올지 정하는 bool들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyType)
	bool isBargest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyType)
	bool isMimic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyType)
	bool isBug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyType)
	bool isTest;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = FSMComp)
	class UDungeonEnemyFSM* FSM;

	//자신 주변에 있는 에너미들을 저장하는 변수
/*	FVector Separate(TArray<AActor*> localEnemy);*/

	UPROPERTY(EditAnywhere, Category = Radius)
	float SeparationRadius = 400.0f;

	UFUNCTION()
	void InteractionPlayer(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION()
	void EnemyOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	//HP UI 관련
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* UIComp;

	class UEnemyUI* EnemyHPUI;

// 	UPROPERTY(Replicated, BlueprintReadOnly, Category = HP)
// 	float hp;

// 	__declspec( property( get = GetHP , put = SetHP ) ) float HP;
// 	float GetHP();
// 	void SetHP(float value);

/*	void OnRep_HP();*/

	
	/*void DamageProcess(int32 damage);*/

	
// 	UFUNCTION()
// 	void CheckEnemy(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

// 	UPROPERTY(EditDefaultsOnly)
// 	class USphereComponent* CheckEnemyComp;
};
