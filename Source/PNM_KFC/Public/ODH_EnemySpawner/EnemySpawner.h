// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PNM_KFC_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//에너미 받는 변수
	UPROPERTY(EditAnywhere, Category = Enemy)
	TSubclassOf<class ATestEnemy> SetEnemy;

	//소환되어 있는 에너미를 저장하는 배열
	UPROPERTY(Editanywhere, Category = EnemyList)
	TArray<class ATestEnemy*> EnemyList;

	//대기(사망)되어 있는 에너미를 저장하는 배열
	UPROPERTY(VisibleAnywhere, Category = EnemyList)
	TArray<class ATestEnemy*> ObjectPool;

	//해당 스포너에서 총 몇마리를 소환할 것인지 정하는 값
	UPROPERTY(EditAnywhere)
	int32 MaxEnemyCount = 2;

	void EnemyOnDamage(int32 listNum);

	void CallEnemyDie(ATestEnemy* enemy);

	void CheckEnemiesValid();

private:
	void SpawnEnemy();
};
