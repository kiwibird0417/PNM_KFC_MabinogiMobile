// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Basic,				// 수동모드(이동, 조작)
	Auto,

	Idle,
	
	AutoQuest,
	AutoTargeting,
	Interacting,
	
	TrackTarget,
	Attack,				// 기본 모드
	Skill,
	Combat,
	
	Damaged,
	Dead,
	AfterDeath,


};



UCLASS()
class PNM_KFC_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


//-------------------------------------------------------------------------------------------
// 0. 플레이어 기본 속성

public:
	//(1). 자동 모드 판별
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerBase)
	bool bIsAuto = false;

	//------------------------------------------------------------------
	//(2) 상태 변수
	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = PlayerFSM)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerFSM)
	//UPROPERTY(ReplicatedUsing = OnRep_mState)
	EPlayerState mState = EPlayerState::Basic;

	UFUNCTION()
	void SetPlayerState(EPlayerState NewState);

	//UFUNCTION(Server, Reliable)
	//void Server_SetPlayerState(EPlayerState NewState);
	//
	//UFUNCTION()
	//void OnRep_mState();




	//------------------------------------------------------------------
public:
	// auto 활성화/비활성화
	void SetAuto();			

	// 특정 상태로 bIsAuto 설정
	void SetAutoState(bool bAutoState);


//---------------------------------------------------------------------------------------------
public:
	// 1. 카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* PlayerCameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

//---------------------------------------------------------------------------------------------
public:
	// 2. 입력 담당 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UAC_PlayerInputComponent* PlayerInputComp;


//---------------------------------------------------------------------------------------------
public:
	// 3. UI
	// 플레이어 메인 UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerMainUI> mainUIWidget;

	UPROPERTY()
	class UPlayerMainUI* mainUI;

	// UI 초기화 함수
	void InitUIWidget();

//---------------------------------------------------------------------------------------------
public:
	// 4. 플레이어 공격
	// 플레이어 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerProperties)
	float MaxHP = 1000;

	// 현재 체력
	UPROPERTY(ReplicatedUsing = OnRep_HP, EditDefaultsOnly, BlueprintReadOnly, Category = PlayerProperties)
	float hp = MaxHP;

	UFUNCTION()
	void OnRep_HP();

	UFUNCTION(Server, Reliable)
	void Server_SetHP(float Value);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_PlayDeadAnimation();

	



	//--------------------------------------------------
	//__declspec : Get, Set 함수 대신 이걸 사용할 수도 있다.
	__declspec( property( get = GetHP , put = SetHP ) ) float HP;
	float GetHP();
	void SetHP(float value);

	
	void BasicState();

	void AutoState();
	void AttackState();

	// 피격 관련 구현
	void DamagedState();

	void ApplyDamagetoPlayer(int32 damage);

	//================================================================

	// 클라이언트에서 호출 → 서버에서 처리
	UFUNCTION(Server, Reliable)
	void Server_ApplyDamage(int32 Damage);

	// 서버에서 모든 클라에 전파
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayDamageEffect();





//---------------------------------------------------------------------------------------------
//5. 상호작용 가능한 오브젝트 추적(타겟팅)
public:
// 타겟팅 관련
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Auto Targeting")
    AActor* CurrentTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auto Targeting")
    float LockOnRange = 600.0f; // 락온 거리

    UPROPERTY(BlueprintReadOnly, Category="Auto Targeting")
    bool bIsLockedOn = false;

    UPROPERTY(BlueprintReadOnly, Category="Auto Targeting")
    bool bIsPlayerMoving = false;

    // 타겟 찾기
    void FindAndLockOnNearestTarget();

    // 타겟 해제
    void UnlockTarget();

	bool canAttack = false;
	bool canInteract = false;

	bool isInteracting = false;

	//UPROPERTY(Replicated)
	bool isAttacking = false;

//---------------------------------------------------------------------------------------------
// 6. 공격 구현하기
public:
	void MoveToTarget(AActor* Target, float StopDistance);

	void LookAtTarget();

	UFUNCTION()
	void Attack();

	//----------------------------------------------------------------
	UFUNCTION(Server, Reliable)
	void ServerRPC_Attack();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_Attack();


	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	// 적을 향해 추적하고 있는가?
	//UPROPERTY(Replicated)
	bool isTrackingTarget = true;

	void TrackTargetState();

	//-------------------------------------------
	FTimerHandle AttackTimerHandle;
	bool bIsAttackCooldown = false;

	void OnAttackFinished();

//---------------------------------------------------------------------------------------------
// 7. 클래스 및 클래스별 스킬 구현하기
public:
	// 클래스 컴포넌트 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = PlayerClass)
	class UAC_PlayerClassComponent* PlayerClassComp;

	//===========================================================================
	//0418(금)
	// 네트워크를 위한 플레이어 자체 클래스 정보를 세팅할 수 있는 곳을 만들어주자.
	//onrep
	UPROPERTY(ReplicatedUsing = OnRep_PlayerClass, EditAnywhere, BlueprintReadWrite, category = PlayerClass)
	//UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, category = PlayerClass)
	//UPROPERTY(EditAnywhere , BlueprintReadWrite , category = PlayerClass)
	EPlayerClassType Playerclass;

	UFUNCTION()
	void SetPlayerClass();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerClass();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayerClass();

	UFUNCTION()
	void OnRep_PlayerClass();


	//============================================================================
	// 플레이어 스킬 UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerSkillUI> skillUIWidget;

	UPROPERTY()
	class UPlayerSkillUI* skillUI;

	// 스킬 컴포넌트 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = PlayerSkill)
	class UAC_PlayerSkillComponent* PlayerSkillComp;

//---------------------------------------------------------------------------------------------
// 8. 플레이어 레벨 구현
public:
	// 현재 플레이어 레벨
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerProperties)
	int32 playerLevel = 1;

	//__declspec : Get, Set 함수 대신 이걸 사용할 수도 있다.
	__declspec( property( get = GetPLAYER_LEVEL , put = SetPLAYER_LEVEL ) ) int PLAYER_LEVEL;
	int GetPLAYER_LEVEL();
	void SetPLAYER_LEVEL(int value);



	//----------------------------------------------
	// 충돌 함수
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

//---------------------------------------------------------------------------------------------
//9. 플레이어 정보 UI 구현
	// 플레이어 정보 UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerInfoUI> infoUIWidget;

	UPROPERTY()
	class UPlayerInfoUI* infoUI;

//---------------------------------------------------------------------------------------------
//10. 플레이어 인벤토리 구현 (무기 장착을 위한)
	// 플레이어 인벤토리 컴포넌트
	// 클래스 컴포넌트 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = PlayerInventory)
	class UAC_PlayerInventoryComponent* PlayerInventoryComp;

	// 골드 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerProperties)
	int32 playerGold = 100;

	//__declspec : Get, Set 함수 대신 이걸 사용할 수도 있다.
	__declspec( property( get = GetPLAYER_GOLD , put = SetPLAYER_GOLD ) ) int PLAYER_GOLD;
	int GetPLAYER_GOLD();
	void SetPLAYER_GOLD(int value);

	// 현재 장착 중인 무기
	UPROPERTY()
	class UWeaponInstance* EquippedWeapon;

public:
	void EquipWeapon(UWeaponInstance* NewWeapon);

	UWeaponInstance* GetEquippedWeapon() const { return EquippedWeapon; }


	// 초기 무기 인벤토리 설정
	void AddInitWeapons();

	// 초기 무기 장착
	void InitEquipWeapon();

	//===============================================================
	//0418(금) 네트워크 무기 장착(Mesh) 반영하려고 해보는 중
	//===============================================================
	
	// 플레이어 무기 구현(외형)
	UPROPERTY(ReplicatedUsing = OnRep_WeaponMesh, EditAnywhere, BlueprintReadWrite, category = Weapon)
	//UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, category = Weapon)
	class UStaticMeshComponent* WeaponComp;

	UFUNCTION()
	void SetWeaponMesh();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetWeaponMesh();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetWeaponMesh(UStaticMesh* NewMesh);

	UFUNCTION()
	void OnRep_WeaponMesh();








	//======================================================================================
	// 클래스별로 다르게	
	// 공격이 시작되는 범위 (정확히는 타겟팅이긴 한데, 이 부분 좀 변경해줘야 할 듯)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = AutoTargeting)
	float AttackRange = 200.f;

public:
	void SetAttackRange(float value);

//---------------------------------------------------------------------------------------------
// 11. 플레이어 스킬 구현
	// 스킬 사용중
	bool bCanUseSkill = false;
	bool bUsingSkill = false;
	bool bSkillEnded = false;

	bool bIsSkillCooldown = false;

	void SkillState(float DeltaTime);


	//UPROPERTY()
	//USkillWidget* SkillWidget; // 위젯 참조

//--------------------------------------------------------------------------------------------
//12. 플레이어 데미지 계산 및 관리 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UAC_PlayerDamageManagerComponent* PlayerDamageManageComp;

	// 플레이어 죽음 구현
	void DeadState();

	// 플레이어가 피격 받기 전 상태 저장
	UPROPERTY()
	EPlayerState tempSaveState;

	// 플레이어 죽음 시 표시되는 UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPlayerDeadUI> deadUIWidget;

	UPROPERTY()
	class UPlayerDeadUI* deadUI;

	void AfterDeathState();
	bool bIsPlayerDead = false;

	bool bShowDeathUI = false;
	void OnDeathAnimationFinished();

//--------------------------------------------------------------------------------------------
//13. 네트워크 연결
	void PrintNetLog();

	// 플레이어들끼리 서로 HP 확인 및 이름 정보 UI
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* netPlayerUI;



	FString GetPlayerClassAsString() const;
	


//--------------------------------------------------------------------------------------------
//14. Session을 통한 방 입장 구현하기
//0423(수)
	virtual void PossessedBy(AController* NewController) override;


//--------------------------------------------------------------------------------------------
//15. 플레이어 이름 정보 불러오기
	// 이름을 네트워크에 복제한다
	UPROPERTY(ReplicatedUsing = OnRep_PlayerName)
	FString playerName;

	void SetNameofPlayer();

// 클라이언트에서 playerName 설정 (RPC)
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerRPC_SetPlayerName(const FString& name);
    void ServerRPC_SetPlayerName_Implementation(const FString& name);
    bool ServerRPC_SetPlayerName_Validate(const FString& name);

	UFUNCTION(NetMulticast , Reliable)
	void MulticastRPC_SetPlayerName(const FString& name);

	// 이름이 변경될 때 호출될 함수
	UFUNCTION()
	void OnRep_PlayerName();

	// 서버에서만 이름을 설정하도록 하는 변수
	bool bHasSetName = false;



//--------------------------------------------------------------------------------------------
//16. Stage 끝
public:
	UPROPERTY(EditAnywhere)
	bool bPlayerEnd = false;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SendMsg(const FString& msg);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SendMsg(const FString& msg);



//--------------------------------------------------------------------------------------------
// 보스 스킬 3 공격을 맞을 경우 필요한 변수와 함수들
// 단해가 만듦
	float HitDelayTime = 0.89f;

	void ApplyTornadoDamagetoPlayer(int32 damage);
};
