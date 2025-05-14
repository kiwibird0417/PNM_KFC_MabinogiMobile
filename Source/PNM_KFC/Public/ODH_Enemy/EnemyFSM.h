// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavigationSystem.h"
#include "EnemyFSM.generated.h"

UENUM()
enum class EEnemyState : uint8
{
	IDLE UMETA(DisplayName = "대기"),
	PATROL UMETA(DisplayName = "이동"),
	CHASE UMETA(DisplayName = "추격"),
	ATTACK UMETA(DisplayName = "공격"),
	DAMAGE UMETA(DisplayName = "데미지"),
	BREAK UMETA(DisplayName = "브레이크"),
	DIE UMETA(DisplayName = "사망")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//최대 체력
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHP = 20.0f;

	//현재 체력
	UPROPERTY(VisibleAnywhere, Category = Stat)
	float CurHP = MaxHP;

	//이동속도
	UPROPERTY(EditAnywhere,Category = FSM)
	float Speed = 200.0f;

	//대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IDLEDelayTime = 3.0f;

	//공격을 하는 공격 범위
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float AttackRange = 500.0f;

	//스포너의 위치를 받기
	UPROPERTY(EditAnywhere, Category = FSM)
	class ADungeonEnemySpawner* Spawner;

	//스포너에서 범위를 제한하는 값
	UPROPERTY(EditAnywhere, Category = FSM)
	float SpawnerRadius = 800.0f;

	//자신의 리스트에서 몇번에 속하는지 저장함
	UPROPERTY(VisibleAnywhere, Category = Enemy)
	int32 ListNum;

private:	
	//자신의 주인을 저장하는 변수
	class AActor* me;

	//플레이어를 저장해주는 변수
	class AActor* Target;

	//경과 시간
	float CurTime = 0.0f;

	//랜덤 값 범위 지정
	int32 RandomRate = 1;

	//목표 위치를 저장하는 변수
	FVector TargetLocation;

	//현재 이동하고 있는지 체크하는 불
	bool bIsPatrol = false;

	//spawner의 위치에서 X의 랜덤 값을 뽑는 함수
	float RandomX(float locationX);

	//spawner의 위치에서 Y의 랜덤 값을 뽑는 함수
	float RandomY(float locationY);

private:	//FSM 관련
	//대기
	void IDLEState();
	//배회
	void PATROLState();
	//추격
	void CAHSEState();
	//공격
	void ATTACKState();
	//피격
	void DAMAGEState();
	//브레이크
	void BREAKState();
	//사망
	void DIEState();

public: //FSM
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::IDLE;
};
