// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "BossFSM.generated.h"

UENUM()
enum class EBossState : uint8
{
	IDLE UMETA(DisplayName = "대기") ,
	CHASE UMETA(DisplayName = "추격"),
	ATTACK UMETA(DisplayName = "공격"),
	RANGEDATTACK UMETA(DisplayName = "원거리공격"),
	DAMAGE UMETA(DisplayName = "데미지"),
	BREAK UMETA(DisplayName = "브레이크"),
	DIE UMETA(DisplayName = "사망")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PNM_KFC_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: //데이터 테이블 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* EnemyDataTable;

	UFUNCTION(BlueprintCallable)
	void LoadStat(FName RowName);

public:
	//최대 체력
	UPROPERTY(ReplicatedUsing = OnRep_HP , EditAnywhere, Category = Stat)
	float MaxHP/* = 20.0f*/;

	//현재 체력
	UPROPERTY(ReplicatedUsing = OnRep_HP, EditAnywhere , Category = Stat)
	float CurHP = MaxHP;

	__declspec( property( get = GetHP , put = SetHP ) ) float HP;
	void SetHP(float value);
	float GetHP();

	UFUNCTION()
	void OnRep_HP();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowClearWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_ShowClearWidget();

	UFUNCTION(BlueprintCallable)
	void ChangeBGM(bool bClear);

	//브레이크 체력
	UPROPERTY(EditAnywhere, Category = Stat)
	float BreakHP/* = 3*/;

	//이동속도
	UPROPERTY(EditAnywhere,Category = Stat)
	float Speed/* = 200.0f*/;

	//공격을 하는 공격 범위
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float AttackRange = 300.0f;

	//경험치
	UPROPERTY(EditAnywhere, Category = Stat)
	int32 EXP;

	//골드
	UPROPERTY(EditAnywhere, Category = Stat)
	int32 Gold;

	//주인의 애니메이션 블루프린트를 저장
	UPROPERTY(EditDefaultsOnly, Category = Anim)
	class UBossAnim* Anim;

	//Easing Function이 실행되는 시간?
	UPROPERTY(EditAnywhere, Category = Anim)
	float FunctionDuration = 15.0f;

	float CurDieTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = Anim)
	float DelayDieTime = 2.0f;

	//추적시간이 길 경우 원거리 공격을 하게 만드는 변수
	UPROPERTY(VisibleAnywhere, Category = Chase)
	float ChaseTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = Chase)
	float MaxChaseTime = 3.0f;

	//FSM
/*	UPROPERTY(Replicated)*/
	EBossState mState = EBossState::IDLE;

	//스킬 3번을 공격할 때 보여줄 파티클
	UPROPERTY(EditDefaultsOnly, Category = Effect)

	TSubclassOf<class AActor> Skill3Effect;

	//타겟팅하는 플레이어를 바꿔주는 함수
	void SetTarget(APlayerCharacter* player);

	//공격과 관련된 함수들
	void StartAttackPlayer();

	void EndAttackPlayer();

	void StartSwingAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SwingCheck();

	UFUNCTION(Netmulticast, Reliable)
	void MulticastRPC_SwingCheck(APlayerCharacter* player);

	void EndSwingAttack();

	void GoToPlayer();

	void StartStompAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StompCheak();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StompCheak(APlayerCharacter* player);

	void Skill3Attack();

	void DieEnemy();

private:
	//자신의 주인(엑터)을 저장하는 변수
	class ABossEnemy* me;

	//플레이어를 저장해주는 변수
	UPROPERTY(Replicated)
	class APlayerCharacter* Target;

	//어택을 했을 경우 카운트를 추가
	int32 AttackCount = 0;
	
	//공격 카운트가 아래 카운트와 같아질 경우 타겟을 변경하게 하는 변수
	UPROPERTY(ReplicatedUsing = OnRep_RandomTargetChange)
	int32 TargetChange = 0;

	UFUNCTION()
	void OnRep_RandomTargetChange();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SearchPlayer(UBossFSM* boss);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SearchPlayer(UBossFSM* boss, APlayerCharacter* player);

	//공격 패턴을 정하는 것을 부르는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetRanNum();

	//공격 패턴을 정하는 것을 부르는 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetRanNum(const int32 num);

	//원거리 공격 패턴을 정하는 것을 부르는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_RangedSetRanNum();

	//원거리 공격 패턴을 정하는 것을 부르는 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RangedSetRanNum(const int32 num);

	class UEnemyUI* EnemyHPUI;

	//공격 패턴을 정하는 랜덤 값 int32
	UPROPERTY()
	int32 RanNum;

	//사망 여부를 확인하는 불
	bool bIsDie;

	//현재 Function이 얼만큼 실행됐는지 체크하는 값
	float FunctionTimer = 0.0f;

	//Ease Function 이동을 해야하는 상황인가 체크하는 불
	bool bEaseFunctionMove = false;

	float Alpha =0.0f;

	//Ease Function이 시작할 때 타겟의 위치를 저장하기 위함
	FVector EasingLocation;

	//타겟 위치로 움직일때 공격범위를 보여줄 액터
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TSubclassOf<AActor> AttackRangeEffect;

public: //스킬 이펙트 관련
	UPROPERTY(EditDefaultsOnly, Category = SkillEffect)
	class UParticleSystem* StompEffect;

public:
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SwingSound;	

private:	//FSM 관련
	//대기
	void IDLEState();
	//추격
	void CHASEState();
	//공격
	void ATTACKState();
	//원거리공격
	void RANGEDATTACKState();
	//피격
	void DAMAGEState();
	//브레이크
	void BREAKState();
	//사망
	void DIEState();

	float EaseOutExpo(float alpha);
};
