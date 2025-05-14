// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestEnemy.h"
#include "Net/UnrealNetwork.h"
#include "DungeonEnemyFSM.generated.h"

UENUM()
enum class EDungeonEnemyState : uint8
{
	IDLE UMETA(DisplayName = "대기"),
	CHASE UMETA(DisplayName = "추격"),
	ATTACK UMETA(DisplayName = "공격"),
	DAMAGE UMETA(DisplayName = "데미지"),
	BREAK UMETA(DisplayName = "브레이크"),
	DIE UMETA(DisplayName = "사망")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UDungeonEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDungeonEnemyFSM();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public: //데이터 테이블 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* EnemyDataTable;

	UFUNCTION(BlueprintCallable)
	void LoadStat(FName RowName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//최대 체력
	UPROPERTY(ReplicatedUsing = OnRep_HP , EditAnywhere, Category = Stat)
	float MaxHP /*= 20.0f*/;
	
	//현재 체력
	UPROPERTY(ReplicatedUsing = OnRep_HP, EditAnywhere, Category = Stat)
	float CurHP/* = MaxHP*/;

	__declspec( property( get = GetHP , put = SetHP ) ) float HP;
	void SetHP(float value);
	float GetHP();

	UFUNCTION()
	void OnRep_HP();

	//현재 체력
	UPROPERTY(Replicated , EditAnywhere, Category = Stat)
	float BreakHP;
	
	//이동속도
	UPROPERTY(EditAnywhere,Category = Stat)
	float Speed/* = 200.0f*/;

	//공격을 하는 공격 범위
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float AttackRange = 220.0f;

	//경험치
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	int32 EXP;

	//돈
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	int32 Gold;

	//주인의 애니메이션 블루프린트를 저장
	UPROPERTY(EditDefaultsOnly, Category = Anim)
	class UEnemyAnim* Anim;

	//스포너의 위치를 받기
	UPROPERTY(EditAnywhere, Category = FSM)
	class ADungeonEnemySpawner* Spawner;

	//플레이어 공격함수
	void StartAttackPlayer();

	//플레이어 공격했을 때 부르는 함수들
	UFUNCTION(Server, Unreliable)
	void ServerRPC_AttackPlayer();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_AttackPlayer();

	//공격 애니메이션이 끝날 경우 state변화 해주는 함수
	void EndAttackPlayer();

	//어택을 했을 경우 카운트를 추가
	int32 AttackCount = 0;

	//공격 카운트가 아래 카운트와 같아질 경우 타겟을 변경하게 하는 변수
	UPROPERTY(ReplicatedUsing = OnRep_RandomTargetChange)
	int32 TargetChange = 0;

	UFUNCTION()
	void OnRep_RandomTargetChange();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SearchPlayer(UDungeonEnemyFSM* enemy);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SearchPlayer(UDungeonEnemyFSM* enemy, APlayerCharacter* player);

	//사망시 몇 초 후에 사라지게 만들어주기 위한 함수
	void DieEnemy();

	UPROPERTY(EditAnywhere, Category = Anim)
	float DieDelayTime = 2.0f;

	float CurDieTime = 0.0f;

	//플레이어를 저장해주는 변수
	UPROPERTY(Replicated)
	class APlayerCharacter* Target;

private:	
	//자신의 주인을 저장하는 변수
	class ATestEnemy* me;

	/*UPROPERTY(ReplicatedUsing = OnRep_HP)*/
	class UEnemyUI* HPUI;

	//현재 플레이어를 추적하는지 확인하는 불
	bool bChasePlayer = false;

	//죽은 상태인지 확인하는 불
	bool bIsDie = false;

private:
	float CheckDist(FVector targetPos, FVector myPos);

private:	//FSM 관련
	//대기
	void IDLEState();
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
	EDungeonEnemyState mState = EDungeonEnemyState::IDLE;

	//라인 트레이스를 통한 좌우 이동을 위해 추가한것

	//라인 트레이스 발사 함수
	void PerformLineTrace();

	//라인 트레이스 타이머 핸들
	FTimerHandle LineTraceTimerHandle;

	//이동 상태
	bool bIsMovingSideway =false;
	bool bMoveRight = false;
	bool bSideRotation = false;

	//좌우 이동할 때 하는 시간 조절
	float CurSideTime = 0.0f;
	float SideTime = 0.8f;

	FVector SideDirection;

	//라인 트레이스 간격
	float LineTraceInterval = 0.9f;

	//라인 트레이스가 발사되는 각도
	float TraceAngle1 = 15.0f;
	float TraceAngle2 = 30.0f;

	TArray<float>Angles = {0.0f, TraceAngle1, -TraceAngle1, TraceAngle2, -TraceAngle2};

	//좌우 이동 지속시간
	float SidewaysMoveDuration = 1.4f;

	//라인 트레이스 거리
	float LineTraceDistance = 400.0f;

public:
	
	UPROPERTY(EditDefaultsOnly, category = Barghest)
	class USoundBase* Barghest_IDLE;

	UPROPERTY(EditDefaultsOnly, category = Barghest)
	class USoundBase* Barghest_Attack;

	UPROPERTY(EditDefaultsOnly, category = Barghest)
	class USoundBase* Barghest_Die;

	UPROPERTY(EditDefaultsOnly, category = Mimic)
	class USoundBase* Mimic_IDLE;

	UPROPERTY(EditDefaultsOnly, category = Mimic)
	class USoundBase* Mimic_Attack;

	UPROPERTY(EditDefaultsOnly, category = Mimic)
	class USoundBase* Mimic_Die;

	UPROPERTY(EditDefaultsOnly, category = Bug)
	class USoundBase* Bug_IDLE;

	UPROPERTY(EditDefaultsOnly, category = Bug)
	class USoundBase* Bug_Attack;

	UPROPERTY(EditDefaultsOnly, category = Bug)
	class USoundBase* Bug_Die;




	//회피 임시로 추가한것

// 	//회피 반경
// 	float AvoidanceRadius = 200.0f;
// 
// 	//회피 힘 가중치
// 	float AvoidanceWeight = 1.0f;
// 
// 	//에너미 간 회피 로직
// 	FVector CalculateAvoidanceForce();
};
