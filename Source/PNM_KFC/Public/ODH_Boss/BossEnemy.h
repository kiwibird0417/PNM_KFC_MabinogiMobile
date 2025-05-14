// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"

UCLASS()
class PNM_KFC_API ABossEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	//FSM
	UPROPERTY(Replicated, EditDefaultsOnly, Category = FSM)
	class UBossFSM* FSM;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* ZPosSceneComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* StompEffecPos;

	//HP UI 관련
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* UIComp;

	UPROPERTY(Replicated)
	class UEnemyUI* EnemyHPUI;

	UPROPERTY(BlueprintReadOnly, Category = HP)
	float hp;

	__declspec( property( get = GetHP , put = SetHP ) ) float HP;
	float GetHP();
	void SetHP(float value);

	void DamageProcess(int32 damage);

public:
	public: //스킬 이펙트 관련
	UPROPERTY(EditDefaultsOnly , Category = SkillEffect)
	class UParticleSystemComponent* AttackParticle;

	UPROPERTY(EditDefaultsOnly, Category = SkillEffect)
	class UParticleSystemComponent* SwingParticle;
	
// 	UPROPERTY(EditDefaultsOnly, Category = SkillEffect)
// 	class UParticleSystemComponent* MovingParticle;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = SkillEffect)
// 	class UParticleSystemComponent* StompParticle;


	

private:
	//플레이어 감지 영역
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

	UFUNCTION()
	void OnDamege(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION()
	void InteractionPlayer(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
