// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyDataTables.generated.h"

USTRUCT(BlueprintType)
struct FEnemyDataTable:public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Data")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Data")
	float BreakHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Data")
	int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Data")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Data")
	int32 EXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Data")
	int32 Gold;
};

UCLASS()
class PNM_KFC_API AEnemyDataTables : public AActor
{
	GENERATED_BODY()
	
	
public:	
	// Sets default values for this actor's properties
	AEnemyDataTables();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
