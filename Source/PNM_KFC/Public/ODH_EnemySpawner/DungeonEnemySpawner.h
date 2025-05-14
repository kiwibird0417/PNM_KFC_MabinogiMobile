// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Door/DoorActor.h"
#include "DungeonEnemySpawner.generated.h"

UCLASS()
class PNM_KFC_API ADungeonEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//에너미가 총 몇개의 웨이브를 가질지 체크하는 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveSetting)
	int32 WaveCount;

	//순차적으로 웨이브마다 얼마만큼 나오는지 값을 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveSetting)
	TArray<int32> EnemyCounts;

	//순차적으로 웨이브마다 어떠한 적이 나오는지 셋팅
	//0은 박쥐
	//1은 쥐
	//2는... 이러한 형식
	UPROPERTY(EditAnywhere, Category = WaveSetting)
	TArray<int32> EnemyType;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnPos1;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnPos2;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnPos3;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnPos4;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnPos5;

	TArray<class USceneComponent*> SpawnPosList;

	//에너미들을 받는 변수
	//먼저 임시로 그냥 액터로 설정
	UPROPERTY(EditAnywhere, Category = EnemyType)
	TSubclassOf<ATestEnemy> BargestFactory;

	UPROPERTY(EditAnywhere, Category = EnemyType)
	TSubclassOf<ATestEnemy> MimicFactory;

	UPROPERTY(EditAnywhere , Category = EnemyType)
	TSubclassOf<ATestEnemy> BugFactory;

	//스폰된 에너미들이 죽었을 경우 불러오는 함수
	void RemoveEnemyList();

	UPROPERTY(EditAnywhere, Category = WaveSetting)
	class ADoorActor* NextDoor;

private:
	//스폰된 에너미들을 저장하는 변수
	UPROPERTY(VisibleAnywhere, Category = WaveSetting)
	TArray<ATestEnemy*> SpawnEnemys;

	//바로 스폰이 되지 않도록 막는 변수
	float CurSpawnTime = 0.0;

	//해당 시간이 지나면 소환되도록 하는 변수값
	float SpawnDelayTime = 0.8f;

	//인덱스 위치를 설정하는 변수
	int32 IndexPoint = 0;

	//에너미를 소환하는 함수
	void WaveSpawn(int32 enemyType, int32 enemyCount);

	UFUNCTION(Server, Reliable)
	void ServerRPC_WaveSpawn(int32 type , int32 count);

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UFUNCTION()
	void OnPlayer(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);
};
