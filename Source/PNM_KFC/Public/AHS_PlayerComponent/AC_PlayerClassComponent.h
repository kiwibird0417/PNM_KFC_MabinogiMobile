// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerSkillData.h"
#include "Net/UnrealNetwork.h"
#include "AC_PlayerClassComponent.generated.h"


UENUM(BlueprintType)
enum class EPlayerClassType : uint8
{
	Warrior,
	Archer,
    Mage,
    Healer,
    Bard,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UAC_PlayerClassComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_PlayerClassComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//--------------------------------------------------------------
public:
	// 플레이어 캐릭터 참조
	class APlayerCharacter* OwnerCharacter;

//--------------------------------------------------------------
public: 
	UPROPERTY(ReplicatedUsing = OnRep_PlayerClass)
	EPlayerClassType PlayerClass;

	UFUNCTION()
	void OnRep_PlayerClass();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(UAC_PlayerClassComponent , PlayerClass);
	}



	UFUNCTION(BlueprintCallable, Category = "Class")
	void SetupClassDefaults();

	UFUNCTION(BlueprintCallable, Category = "Class")
	void SetupClassDefaults_Logic();

//--------------------------------------------------------------
public:
	// 데이터테이블 변수

	// 해당 함수를 실행하면, 클래스가 바뀐다.
	// 해당 함수에서는, 바뀐 클래스에 맞는 데이터테이블을 찾아서 할당해준다.
	// 할당한 다음에, 데이터테이블에 있는 SkillName만 출력한다.

	// 클래스별로 필터링된 스킬 리스트
	UPROPERTY()
	TArray<FPlayerSkillData> ClassSkills;



		
};
