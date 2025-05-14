#include "AHS_Player/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AHS_PlayerComponent/AC_PlayerInputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "AHS_UI/PlayerMainUI.h"
#include "AHS_Interactables/AC_InteractableComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AHS_UI/PlayerSkillUI.h"
#include "AHS_PlayerComponent/AC_PlayerSkillComponent.h"
#include "Components/CapsuleComponent.h"
#include "AHS_UI/PlayerInfoUI.h"
#include "AHS_PlayerComponent/AC_PlayerInventoryComponent.h"
#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"
#include "AHS_Player/PlayerAnimInstance.h"
#include "AHS_PlayerComponent/AC_PlayerDamageManagerComponent.h"
#include "AHS_UI/PlayerDeadUI.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Boss/BossEnemy.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Actor.h"
#include "PNM_KFC.h"
#include "Components/WidgetComponent.h"
#include "AHS_UI/Player_Multi/Net_PlayerUI.h"
#include "Net/UnrealNetwork.h"
#include "ODH_Enemy/DungeonEnemyFSM.h"
#include "GameFramework/Controller.h"
#include "AHS_Network/NetPlayerController.h"
#include "AHS_Network/NetGameInstance.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//--------------------------------------------------------------------------------
	//1. 카메라 붙이기
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->TargetArmLength = 300.0f;


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; 

	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	SpringArmComp->SetRelativeRotation(FRotator(0.0f, -25.0f, 0.0f));
	SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//***********************************************************//
	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCameraComp"));
	PlayerCameraComp->SetupAttachment(SpringArmComp);
	PlayerCameraComp->bUsePawnControlRotation = false;

	//--------------------------------------------------------------------------------
	// 2. 플레이어 입력 컴포넌트 추가
	PlayerInputComp = CreateDefaultSubobject<UAC_PlayerInputComponent>(TEXT("PlayerInputComp"));
	//--------------------------------------------------------------------------------
	// 3. 플레이어 외형
	if (GetMesh())
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Temp_SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

		if (Temp_SkeletalMesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(Temp_SkeletalMesh.Object);
			GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
			GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		}
	}

	// 3-1. 플레이어 외형(클래스에 따라 무기 추가)
	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	WeaponComp->SetupAttachment(GetMesh() , TEXT("AddWeapon"));
	WeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//--------------------------------------------------------------------------------
	// 4. 플레이어 클래스 컴포넌트 추가(스킬)
	PlayerClassComp = CreateDefaultSubobject<UAC_PlayerClassComponent>(TEXT("PlayerClassComp"));
	PlayerSkillComp = CreateDefaultSubobject<UAC_PlayerSkillComponent>(TEXT("PlayerSkillComp"));
	

	//--------------------------------------------------------------------------------
	// 5. 플레이어 Body Collision(충돌 처리) : 현재는 그냥 Custom으로 처리중
	// PlayerBody로 설정하면, 바닥 통과중이라...
	// pawn --> EnemyAttack Overlap 처리 되도록
	// GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerBody"));


	//--------------------------------------------------------------------------------
	// 6. 플레이어 인벤토리 컴포넌트 추가
	PlayerInventoryComp = CreateDefaultSubobject<UAC_PlayerInventoryComponent>(TEXT("PlayerInventoryComp"));

	//--------------------------------------------------------------------------------
	// 7. 플레이어 데미지 계산 컴포넌트 추가
	PlayerDamageManageComp = CreateDefaultSubobject<UAC_PlayerDamageManagerComponent>(TEXT("PlayerDamageManageComp"));

	//--------------------------------------------------------------------------------
	// 8. 네트워크 연결 ~ 다른 플레이어들이 볼 수 있도록 만든 HP 및 정보 UI
	netPlayerUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("NetPlayerUI"));
	netPlayerUI->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//----------------------------------------------------------------------
	bIsPlayerDead = false;
	
	// 0. UI 추가
	InitUIWidget();

	//----------------------------------------------------------------------
	// 0 - 1. 플레이어 이름 설정
	// 게임 인스턴스에서 이름을 가져와서 클라이언트가 서버에 전송할 수 있도록 설정

	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if ( gi )
	{
		FString inputName = *gi->mySessionName;

		// 서버에 이름을 전송
		if ( HasAuthority() )
		{
			playerName = inputName;  // 서버 측에서 이름 설정
			OnRep_PlayerName();  // 서버에서 UI에 이름 반영
			MulticastRPC_SetPlayerName(playerName);  // 모든 클라이언트에 이름 전파
		}
		else
		{
			// 클라이언트 측에서는 서버에 이름을 보내기
			ServerRPC_SetPlayerName(inputName);
		}
	}
	//----------------------------------------------------------------------


	// 1. 한 번 초기화해서 데이터 테이블을 불러와서 스킬이랑 연동해주고, 시작
	OnRep_PlayerClass();
	SetPlayerClass();

	// 2. CapsuleComponent Collision 설정 추가
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this , &APlayerCharacter::OnCapsuleBeginOverlap);

	//-----------------------------------------------------------
	// 3. 플레이어 HP Max로 설정해서 시작
	// 체력 최대치로, 레벨 갱신
	SetHP(MaxHP);
	SetPLAYER_LEVEL(playerLevel);
	SetPLAYER_GOLD(playerGold);

	//-----------------------------------------------------------
	// 4. 초기 무기 인벤토리에 저장
	AddInitWeapons();

	// 초기 무기 장착
	InitEquipWeapon();





	//-----------------------------------------------------------
	// 단해가 추가한 코드

	HitDelayTime = 0.0f;
	//---------------------------------------------------------- -
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어 HP가 0보다 작다면 mState = Dead
	if ( GetHP() <= 0 ) {
		//mState = EPlayerState::Dead;
		SetPlayerState(EPlayerState::Dead);
	}

	//=========================================================================
	// 상대방 플레이어를 향하도록, UI 빌보드
	if ( netPlayerUI && netPlayerUI->GetVisibleFlag() ) {
		FVector CamLoc = UGameplayStatics::GetPlayerCameraManager(GetWorld() , 0)->GetCameraLocation();

		FVector Direction = CamLoc - netPlayerUI->GetComponentLocation();
		Direction.Z = 0.0f;

		netPlayerUI->SetWorldRotation(Direction.GetSafeNormal().ToOrientationRotator());
	}


	//-------------------------------------------------------------------------
	// 실행창에 상태 메세지 출력하기
	//FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(100 , 10 , FColor::Red , logMsg);


	// ...
	switch ( mState )
	{
		case EPlayerState::Basic: { BasicState(); }	break;
		case EPlayerState::Auto: { AutoState(); }	break;
		case EPlayerState::TrackTarget: { TrackTargetState(); }	break;
		case EPlayerState::Attack: { AttackState(); }	break;

		case EPlayerState::Skill: { SkillState(DeltaTime); } break;

		// 피격 상태라면,
		case EPlayerState::Damaged:{ DamagedState(); } break;
		case EPlayerState::Dead:{ DeadState(); } break;
		case EPlayerState::AfterDeath: { AfterDeathState(); } break;
	}
	//----------------------------------------------------------------------
	// 공격중이라면, 새로운 타겟을 탐색하지 않는다.
	if ( isAttacking ) {
		return;
	}

	//----------------------------------------------------------------------
	// 타겟 탐색
	FindAndLockOnNearestTarget();


	if ( CurrentTarget )
	{
		float Distance = FVector::Dist(GetActorLocation() , CurrentTarget->GetActorLocation());
		if ( Distance > LockOnRange )
		{
			UnlockTarget();
		}
		else {
			ATestEnemy* enemy = Cast<ATestEnemy>(CurrentTarget);
			ABossEnemy* boss = Cast<ABossEnemy>(CurrentTarget);

			if ( enemy != nullptr && boss == nullptr )
			{
				if ( enemy->FSM->GetHP() <= 0 )
				{
					// Overlap 대상들 담을 배열
					TArray<FOverlapResult> Overlaps;
				
					// 충돌 쿼리 설정
					FCollisionQueryParams QueryParams;
					QueryParams.AddIgnoredActor(this); // 자기 자신 무시
					QueryParams.bTraceComplex = false;
				
					// 충돌 채널 설정 (적절한 채널로 변경 가능)
					FCollisionObjectQueryParams ObjectQueryParams;
					ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn); // Pawn 채널에 적들이 설정되어 있다면
				
					// Overlap 실행
					bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
						Overlaps ,
						GetActorLocation() ,                    // 플레이어 위치 기준
						FQuat::Identity ,                       // 회전 없음
						ObjectQueryParams ,
						FCollisionShape::MakeSphere(LockOnRange) ,
						QueryParams
					);
				
					bool bFoundOtherTarget = false;
				
					for ( auto& Overlap : Overlaps )
					{
						AActor* OverlappedActor = Overlap.GetActor();
						if ( OverlappedActor && OverlappedActor != enemy )
						{
							if ( Cast<ATestEnemy>(OverlappedActor) || Cast<ABossEnemy>(OverlappedActor) )
							{
								bFoundOtherTarget = true;
								break;
							}
						}
					}
				
					if ( !bFoundOtherTarget )
					{
						UnlockTarget();

						// 주변에 적이 없다면.
						bIsAuto = false;

						SetPlayerState(EPlayerState::Basic);
						//mState = EPlayerState::Basic;
					}
				}
			}
		}
	}

	//----------------------------------------------------------------------
	// 이동 여부 체크 (velocity 기반)
	bIsPlayerMoving = GetVelocity().Size() > 5.0f; // 작게 남는 움직임 잡기 위해 임계값 설정

	//----------------------------------------------------------------------
	//PrintNetLog();


	//----------------------------------------------------------------------
	// 단해가 추가한 코드
	if ( HitDelayTime >= 0.0f )
	{
		HitDelayTime -= DeltaTime;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (PlayerInputComp)
		{
			PlayerInputComp->SetupInputComponent(PlayerController, PlayerInputComponent);
		}
	}

}

//===================================================================================
// 0422(화)
// state 변경을 network상으로 서로 다르게 출력되고 있는 걸 확인하기 위한 함수!

void APlayerCharacter::SetPlayerState(EPlayerState NewState)
{

	mState = NewState;
}


//--------------------------------------------------------------------------------
// 플레이어 상태 설정(bIsAuto : 자동 모드)
void APlayerCharacter::SetAuto() {
	if ( bIsAuto ) {
		bIsAuto = false;
	}
	else {
		bIsAuto = true;
	}
}

void APlayerCharacter::SetAutoState(bool bAutoState)
{
	bIsAuto = bAutoState;
}


//--------------------------------------------------------------------------------
// 플레이어 UI 생성
void APlayerCharacter::InitUIWidget()
{
	// 멀티 모드(내가 제어 X, 위젯 생성 X)
	auto pc = Cast<APlayerController>(Controller);
	if ( pc == nullptr ) {
		UE_LOG(LogTemp, Warning, TEXT("Net Player Controller is Null!"));
		return;
	}

	//------------------------------------------------
	// Anim Notify에서 수정되는 값, true 이후에 죽음 UI 띄움.
	bShowDeathUI = false;

	//------------------------------------------------
	if ( mainUIWidget ) {
		mainUI = Cast<UPlayerMainUI>(CreateWidget(GetWorld() , mainUIWidget));
		mainUI->AddToViewport();
		mainUI->SetOwnerCharacter(this);
	}

	//if ( pc->mainUIWidget ) {
	//	mainUI = Cast<UPlayerMainUI>(CreateWidget(GetWorld() , mainUIWidget));
	//	
	//	//mainUI = pc->mainUI;
	//
	//	mainUI->AddToViewport();
	//	mainUI->SetOwnerCharacter(this);
	//}

	if ( skillUIWidget ) {
		skillUI = Cast<UPlayerSkillUI>(CreateWidget(GetWorld() , skillUIWidget));
		if ( skillUI ) {
			skillUI->AddToViewport();

			// 투명하게 시작 + 버튼 입력 막기
			skillUI->SetVisibility(ESlateVisibility::Hidden);

		}
	}

	if ( infoUIWidget ) {
		infoUI = Cast<UPlayerInfoUI>(CreateWidget(GetWorld() , infoUIWidget));
		infoUI->AddToViewport();
	}

	//-------------------------------------------------------------------------
	//네트워크 상 다른 사람들에게 보여주는 UI이므로, 플레이어(나 자신)는 볼 수 없도록 처리
	if ( netPlayerUI ) {
		netPlayerUI->SetVisibility(false);
	}

}


//--------------------------------------------------------------------------------
// [플레이어 체력 관련 설정]

float APlayerCharacter::GetHP()
{
	return hp;
}

void APlayerCharacter::SetHP(float value)
{
	Server_SetHP(value);
}

void APlayerCharacter::Server_SetHP_Implementation(float Value)
{
	hp = Value;

	OnRep_HP();
}

void APlayerCharacter::MulticastRPC_PlayDeadAnimation_Implementation()
{
	auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->PlayDeadAnimation();
}

void APlayerCharacter::OnRep_HP()
{
	if ( infoUI ) {
		// 사망 처리
		if ( HP <= 0 ) {
			bIsPlayerDead = true;


			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

		}

		//----------------------------------------------------
		float percent = hp / MaxHP;

		// 메인 화면에 보이는 플레이어 정보 UI라면,
		if ( infoUI ) {
			infoUI->HP = percent;
		}
		// 상대방에게 보일 UI라면...
		else {
			auto hpUI = Cast<UNet_PlayerUI>(netPlayerUI->GetWidget());
			hpUI->HP = percent;
		}
	}
}



//==========================================================================
// 플레이어 데미지 함수(피격)
void APlayerCharacter::ApplyDamagetoPlayer(int32 damage)
{
	// 클라이언트일 경우 서버에 데미지 적용 요청
	if ( !HasAuthority() )
	{
		Server_ApplyDamage(damage);
		return;
	}

	// 서버일 경우, 실제 데미지 처리 + 연출 전파
	int32 playerNewHP = GetHP() - damage;
	SetHP(playerNewHP);

	// 연출 전파
	Multicast_PlayDamageEffect();

}

void APlayerCharacter::Server_ApplyDamage_Implementation(int32 Damage)
{
	ApplyDamagetoPlayer(Damage);
}

void APlayerCharacter::Multicast_PlayDamageEffect_Implementation()
{
	if ( hp <= 0 ) {
		auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		anim->PlayDeadAnimation();
	}
	else {
		auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		anim->PlayDamagedAnimation();
	}
}

// 피격 받았을 때
void APlayerCharacter::DamagedState()
{
	
}




//--------------------------------------------------------------------------------
// [ 플레이어 레벨 관련 설정]
int APlayerCharacter::GetPLAYER_LEVEL()
{
	return playerLevel;
}

void APlayerCharacter::SetPLAYER_LEVEL(int value)
{
	playerLevel = value;

	// UI에 레벨 정보 갱신
	if ( infoUI ) {
		infoUI->SetPlayerLevelText(playerLevel);
	}
}

//--------------------------------------------------------------------------------
// State 구분(Player State Machine)
// 일반(수동 모드)
void APlayerCharacter::BasicState()
{
	// 스킬 사용 UI 막기
	if ( skillUI ) {
		skillUI->SetVisibility(ESlateVisibility::Hidden);
	}

	bCanUseSkill = false;
	bSkillEnded = false;

	// 자동모드라면,
	if ( bIsAuto == true) {
		SetPlayerState(EPlayerState::Auto);
		//mState = EPlayerState::Auto;
	}
}

void APlayerCharacter::AutoState()
{
	// 자동 모드가 아니라면, 다시 기본 상태로 돌아감
	if ( bIsAuto == false ) {
		SetPlayerState(EPlayerState::Basic);
	}


	// 에너미 추적 가능한 상태라면 추적!
	if ( isTrackingTarget ) {
		SetPlayerState(EPlayerState::TrackTarget);
	}
}

void APlayerCharacter::AttackState()
{
	// 0416(프로토 기준)
	// 에너미가 죽으면(nullptr), Basic 상태로 돌아간다.
	if ( CurrentTarget == nullptr ) {
		SetPlayerState(EPlayerState::Basic);
		//mState = EPlayerState::Basic;
	}

	//-----------------------------------

	// 스킬 끝난 후, 리셋
	bSkillEnded = false;

	//--------------------------------------------------------------------
	// 스킬 사용 가능하도록 함.
	bCanUseSkill = true;

	// 스킬을 사용하고 있다면, Skill State로 변경
	if ( bCanUseSkill && bUsingSkill ) {
		SetPlayerState(EPlayerState::Skill);
		//mState = EPlayerState::Skill;
	}

	//--------------------------------------------------------------------

	LookAtTarget();

	//--------------------------------------------------------------------
	// 스킬 사용 UI 보여주기
	if ( skillUI )
	{
		skillUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("skillUI가 nullptr입니다! 초기화 되었는지 확인하세요."));
	}


	if ( bIsAttackCooldown ) return;

	Attack();

	bIsAttackCooldown = true;

	// 3초 뒤에 상태를 Basic으로 돌리고 쿨다운 초기화
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle , this , &APlayerCharacter::OnAttackFinished , 3.0f , false);
}


//--------------------------------------------------------------------------------
// 타겟팅 구현
void APlayerCharacter::FindAndLockOnNearestTarget()
{


	float ClosestDistance = LockOnRange;
	AActor* ClosestTarget = nullptr;

	// 월드 내의 모든 액터 중 타겟 가능한 가장 가까운 대상 찾기
	for ( TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr )
	{
		AActor* Actor = *ActorItr;
		if ( Actor == this ) continue; // 자기 자신 제외

		UAC_InteractableComponent* TargetableComp = Actor->FindComponentByClass<UAC_InteractableComponent>();
		if ( TargetableComp && TargetableComp->bCanTarget )
		{
			float Distance = FVector::Dist(GetActorLocation() , Actor->GetActorLocation());
			if ( Distance < ClosestDistance )
			{
				ClosestDistance = Distance;
				ClosestTarget = Actor;
			}
		}
	}

	// 더 가까운 타겟이 있으면 갱신
	if ( ClosestTarget && ClosestTarget != CurrentTarget )
	{
		CurrentTarget = ClosestTarget;

		//----------------------------------------------------------
		ATestEnemy* enemy = Cast<ATestEnemy>(CurrentTarget);
		ABossEnemy* boss = Cast<ABossEnemy>(CurrentTarget);
		
		if ( enemy != nullptr ) {
			if ( enemy->FSM->GetHP() <= 0 ) {
				CurrentTarget = nullptr;

				SetPlayerState(EPlayerState::Basic);
				//mState = EPlayerState::Basic;
				return;
			}
		}

		if ( boss != nullptr ) {
			if ( boss->GetHP() <= 0 ) {
				CurrentTarget = nullptr;

				SetPlayerState(EPlayerState::Basic);
				//mState = EPlayerState::Basic;
				return;
			}
		}



		//----------------------------------------------------------
		bIsLockedOn = true;

		if ( UAC_InteractableComponent* InteractableComp = CurrentTarget->FindComponentByClass<UAC_InteractableComponent>() )
		{
			bool bEnemy = InteractableComp->bIsEnemy;
			canAttack = bEnemy;
			canInteract = !bEnemy;
		}

		// UI 버튼 이미지 갱신
		if ( mainUI )
		{
			mainUI->ChangeInterActionButtonImage(bIsAuto);
		}

		// 디버그 로그
		UE_LOG(LogTemp , Log , TEXT("Target Locked: %s") , *CurrentTarget->GetName());

		SetPlayerState(EPlayerState::TrackTarget);
		//mState = EPlayerState::TrackTarget;
		
	}

	// 만일 current target의 hp가 0보다 작다면 nullptr로 설정하여 더할 수 없게 설정

}


// 타겟 해제(일정 거리 이상 멀어진다면...)
void APlayerCharacter::UnlockTarget()
{
	if ( CurrentTarget )
	{
		UE_LOG(LogTemp , Log , TEXT("Target Unlocked: %s") , *CurrentTarget->GetName());
	}

	CurrentTarget = nullptr;
	bIsLockedOn = false;

	// UI 갱신
	canAttack = false;
	canInteract = false;
	if ( mainUI )
	{
		mainUI->ChangeInterActionButtonImage(bIsAuto);
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("mainUI is null when calling ChangeInterActionButtonImage"));
	}

	// TODO: UI 락온 해제 등 처리
}

//--------------------------------------------------------------------------------
// 타겟을 향해 이동하기
void APlayerCharacter::MoveToTarget(AActor* Target , float StopDistance)
{
	if ( !bIsAuto || !Target ) return;

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation , TargetLocation);

	if ( Distance > StopDistance )
	{
		FVector Direction = ( TargetLocation - CurrentLocation ).GetSafeNormal();
		AddMovementInput(Direction , 1.0f);
	}
}

// 원본 : 타겟을 향해 회전시키기
void APlayerCharacter::LookAtTarget()
{
	if ( CurrentTarget )
	{
		FVector PlayerDir = CurrentTarget->GetActorLocation() - GetActorLocation();
		PlayerDir.Z = 0.0f;
		PlayerDir.Normalize();

		FRotator TargetRot = PlayerDir.Rotation();
		FRotator CurrentRot = GetActorRotation();

		// 회전 속도 (값이 클수록 빠르게 회전)
		float RotationSpeed = 5.0f;

		float DeltaTime = GetWorld()->GetDeltaSeconds();

		// 부드럽게 회전 (Actor)
		FRotator SmoothRot = FMath::RInterpTo(CurrentRot , TargetRot , DeltaTime , RotationSpeed);
		SetActorRotation(SmoothRot);

		// 부드럽게 회전 (Controller)
		if ( Controller )
		{
			FRotator ControllerRot = Controller->GetControlRotation();

			// 기본 컨트롤러 회전은 TargetRot으로 설정하되,
			// Pitch를 위에서 내려다보게 하기 위해 살짝 꺾어줌
			FRotator AdjustedTargetRot = TargetRot;
			AdjustedTargetRot.Pitch = -20.f; // 약간 위에서 내려다보는 각도
			AdjustedTargetRot.Yaw -= 15.f;   // 살짝 사선에서 볼 수 있도록

			FRotator SmoothControllerRot = FMath::RInterpTo(ControllerRot , AdjustedTargetRot , DeltaTime , RotationSpeed);

			// 컨트롤러 갱신 -> 카메라 각도 갱신
			Controller->SetControlRotation(SmoothControllerRot);
		}
	}
}


//--------------------------------------------------------------------------------
// 공격
void APlayerCharacter::Attack()
{
	ServerRPC_Attack();
}

void APlayerCharacter::ServerRPC_Attack_Implementation()
{
	MulticastRPC_Attack();
}

void APlayerCharacter::MulticastRPC_Attack_Implementation()
{
	auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->Play_BasicAttack_Animation();
}

// 타겟 추적
void APlayerCharacter::TrackTargetState()
{
	// 공격 가능하다면, 공격 가능한 상태가 된다.
	if ( isAttacking ) {
		SetPlayerState(EPlayerState::Attack);
		return;
	}

	//---------------------------------------------------
	// 타겟이 없고, 자동모드가 아니라면 일반 수동 모드로 전환
	// 주변을 검색하여 갱신했는데도, 주위에 없다면 기본 상태로 돌아감.

	if ( CurrentTarget == nullptr || !bIsAuto) {
		SetPlayerState(EPlayerState::Basic);
		return;
	}

	//---------------------------------------------------

	float Distance = FVector::Dist(GetActorLocation() , CurrentTarget->GetActorLocation());
	//UE_LOG(LogTemp , Warning , TEXT("Distance to Target: %f") , Distance);

	if ( Distance > AttackRange )
	{
		MoveToTarget(CurrentTarget , AttackRange);
	}
	else
	{
		// 공격 가능하면 공격 상태로 전환
		if ( canAttack && !isAttacking )
		{
			SetPlayerState(EPlayerState::Attack);
		}
	}
}

// 공격 완료
void APlayerCharacter::OnAttackFinished()
{
	bIsAttackCooldown = false;

	// 공격 후에는 다시 원래 상태로 돌아간다
	isAttacking = false;

	if ( IsLocallyControlled() ) {
		//UE_LOG(LogTemp, Warning, TEXT("Is Attacking is %d"));
	}
}


//================================================================
//0418(금) 네트워크
// 플레이어 클래스 변경 후 반영
void APlayerCharacter::OnRep_PlayerClass()
{
	PlayerClassComp->SetupClassDefaults();
	Playerclass = PlayerClassComp->PlayerClass;
}

void APlayerCharacter::SetPlayerClass()
{
	ServerRPC_PlayerClass();

	PlayerClassComp->SetupClassDefaults();
	Playerclass = PlayerClassComp->PlayerClass;
}



void APlayerCharacter::ServerRPC_PlayerClass_Implementation()
{
	MulticastRPC_PlayerClass();
}

void APlayerCharacter::MulticastRPC_PlayerClass_Implementation()
{
	PlayerClassComp->SetupClassDefaults();
	Playerclass = PlayerClassComp->PlayerClass;
}

//--------------------------------------------------------------------------------
// 충돌 구현
void APlayerCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	if ( OtherComp )
	{
		// Enemy라는 이름을 가진 액터 또는 컴포넌트가 있다면, 충돌 처리가 난다.
		if ( OtherComp->GetName().Contains(TEXT("Enemy")) )
		{
			UE_LOG(LogTemp , Warning , TEXT("충돌!"));
			
			// Damage 로직 실행(임시, 10으로 고정)
			ApplyDamagetoPlayer(10);

		}
	}
}


//--------------------------------------------------------------------------------
// [플레이어 골드 정보 관련]
int APlayerCharacter::GetPLAYER_GOLD()
{
	return playerGold;
}

void APlayerCharacter::SetPLAYER_GOLD(int value)
{
	playerGold = value;

	// UI에 골드 정보 갱신
	if ( infoUI ) {
		infoUI->SetPlayerGoldText(playerGold);
	}
	
}

//--------------------------------------------------------------------------------
// 플레이어 무기 장착
void APlayerCharacter::EquipWeapon(UWeaponInstance* NewWeapon)
{
	if ( NewWeapon == nullptr || NewWeapon == EquippedWeapon )
	{
		return;
	}

	// 이전 무기 장착 해제 로직 (필요시)
	if ( EquippedWeapon )
	{
		EquippedWeapon->OnUnequip(); // 이런 식으로 커스텀 함수 넣을 수도 있음
	}

	EquippedWeapon = NewWeapon;
	EquippedWeapon->OnEquip(); // 이 함수에서 이펙트 재생이나 Mesh 장착 같은 것 처리

	//======================================================================================================
	// 0418(금)
	// 무기 장착을 네트워크 반영용으로 바꾼다(이 부분부터...시도중)
	OnRep_WeaponMesh();
	OnRep_PlayerClass();

	SetWeaponMesh();
	SetPlayerClass();

	//=======================================================================================================
	// 디버깅용
	UE_LOG(LogTemp , Log , TEXT("무기 장착됨: %s") , *EquippedWeapon->GetWeaponName().ToString());
}


//================================================================================================================================
void APlayerCharacter::SetWeaponMesh()
{
	ServerRPC_SetWeaponMesh();

	if ( EquippedWeapon )
	{
		UStaticMesh* FinalMesh = const_cast<UStaticMesh*>( EquippedWeapon->GetFinalMesh(EquippedWeapon->CachedWeaponData) );
		if ( FinalMesh )
		{
			WeaponComp->SetStaticMesh(FinalMesh); // 전달된 NewMesh 사용
		}
		else
		{
			UE_LOG(LogTemp , Warning , TEXT("NewMesh is null!"));
		}
	}

}

void APlayerCharacter::ServerRPC_SetWeaponMesh_Implementation()
{
	if ( EquippedWeapon )
	{
		UStaticMesh* FinalMesh = const_cast<UStaticMesh*>( EquippedWeapon->GetFinalMesh(EquippedWeapon->CachedWeaponData) );
		MulticastRPC_SetWeaponMesh(FinalMesh);
	}
}

void APlayerCharacter::MulticastRPC_SetWeaponMesh_Implementation(UStaticMesh* NewMesh)
{
	if ( !WeaponComp )
	{
		UE_LOG(LogTemp , Warning , TEXT("WeaponComp is null!"));
		return;
	}

	if ( NewMesh )
	{
		WeaponComp->SetStaticMesh(NewMesh); // 전달된 NewMesh 사용
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("NewMesh is null!"));
	}
}


void APlayerCharacter::OnRep_WeaponMesh()
{
	if ( !EquippedWeapon )
	{
		UE_LOG(LogTemp , Error , TEXT("EquippedWeapon is null in OnRep_WeaponMesh"));
		return;
	}

	const UStaticMesh* FinalMesh = EquippedWeapon->GetFinalMesh(EquippedWeapon->CachedWeaponData);
	if ( !FinalMesh )
	{
		UE_LOG(LogTemp , Error , TEXT("FinalMesh is null in OnRep_WeaponMesh"));
		return;
	}

	MulticastRPC_SetWeaponMesh(const_cast<UStaticMesh*>( FinalMesh ));
}

//=============================================================================================================================




// 초기 무기 인벤토리 (디버깅 테스트용)
void APlayerCharacter::AddInitWeapons()
{
	if ( !PlayerInventoryComp || !PlayerInventoryComp->WeaponDataTable ) return;

	// 무기 ID 리스트
	TArray<FName> WeaponIDs = { TEXT("1"), TEXT("1"), TEXT("1"), TEXT("9"), TEXT("9") };

	for ( const FName& WeaponID : WeaponIDs )
	{
		// 무기 데이터 가져오기
		const FWeaponData* RowData = PlayerInventoryComp->WeaponDataTable->FindRow<FWeaponData>(WeaponID , TEXT("Lookup WeaponData"));

		if ( RowData )
		{
			UWeaponInstance* NewWeapon = NewObject<UWeaponInstance>(PlayerInventoryComp);
			NewWeapon->InitializeInstance(WeaponID , *RowData);

			PlayerInventoryComp->AddWeaponInstance(NewWeapon);
		}
		else
		{
		}
	}
}

// 초기 무기 장착
void APlayerCharacter::InitEquipWeapon()
{
	if ( !PlayerInventoryComp ) return;

	const TArray<UWeaponInstance*>& WeaponList = PlayerInventoryComp->GetAllWeapons();

	if ( WeaponList.Num() > 0 )
	{
		EquipWeapon(WeaponList[0]); // 첫 번째 무기를 장착
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("장착할 무기가 없습니다."));
	}
}


// 플레이어 클래스에 따른, 공격 Distance(범위) 조정 함수
void APlayerCharacter::SetAttackRange(float value)
{
	AttackRange = value;
}


//--------------------------------------------------------------------------------
// 스킬 사용
void APlayerCharacter::SkillState(float DeltaTime)
{
	if ( bSkillEnded || !bUsingSkill) {
		SetPlayerState(EPlayerState::Attack);
	}
}


//--------------------------------------------------------------------------------
// 플레이어 죽음
void APlayerCharacter::DeadState()
{
}

void APlayerCharacter::OnDeathAnimationFinished()
{
	if ( bShowDeathUI && mState != EPlayerState::AfterDeath )
	{
		if ( deadUIWidget )
		{
			deadUI = Cast<UPlayerDeadUI>(CreateWidget(GetWorld() , deadUIWidget));
			deadUI->AddToViewport();
		}

		SetPlayerState(EPlayerState::AfterDeath);
	}
}


void APlayerCharacter::AfterDeathState()
{

}


//==============================================================================
// 자기 자신의 클래스(직업) 출력을 위한 임시 함수
FString APlayerCharacter::GetPlayerClassAsString() const
{
	switch ( Playerclass )
	{
	case EPlayerClassType::Warrior:
		return TEXT("Warrior");
	case EPlayerClassType::Mage:
		return TEXT("Mage");
	case EPlayerClassType::Archer:
		return TEXT("Archer");
	default:
		return TEXT("Unknown");
	}
}

//--------------------------------------------------------------------------------
// 13. 네트워크 연결
void APlayerCharacter::PrintNetLog()
{
	FString StateStr = StaticEnum<EPlayerState>()->GetNameStringByValue((int64)mState);


	const FString conStr = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");
	const FString ownerName = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");

	const FString logStr = FString::Printf(
		TEXT("Connection : %s \nOwner Name : %s\nLocalRole : %s\nRemote Role : %s\nPlayer Class : %s\nMState : %s") ,
		*conStr ,
		*ownerName ,
		*LOCAL_ROLE ,
		*REMOTE_ROLE ,
		*GetPlayerClassAsString(),
		*StaticEnum<EPlayerState>()->GetNameStringByValue((int64)mState)
	);


	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 100.0f, logStr, nullptr, FColor::White, 0, true);
}


//===============================================================================

void APlayerCharacter::PossessedBy(AController* NewController)
{
	PRINTLOG(TEXT("Begin"));

	Super::PossessedBy(NewController);

	PRINTLOG(TEXT("End"));
}


// 클라이언트에서 이름을 서버에 전송 (RPC)
void APlayerCharacter::ServerRPC_SetPlayerName_Implementation(const FString& name)
{
	playerName = name;
	OnRep_PlayerName();
	MulticastRPC_SetPlayerName(playerName);  // 모든 클라이언트에 이름 전파
}

bool APlayerCharacter::ServerRPC_SetPlayerName_Validate(const FString& name)
{
	return true;  // 기본적으로 유효성 검사 없음
}

// 모든 클라이언트에서 이름을 설정 (Multicast RPC)
void APlayerCharacter::MulticastRPC_SetPlayerName_Implementation(const FString& name)
{
	playerName = name;

	// 모든 클라이언트의 UI에 이름을 설정
	if ( netPlayerUI )
	{
		UUserWidget* widget = netPlayerUI->GetUserWidgetObject();
		if ( UNet_PlayerUI* playerUI = Cast<UNet_PlayerUI>(widget) )
		{
			playerUI->SetPlayerName(name);  // UI에 playerName을 설정
		}
	}
}

//--------------------------------------------------------------------------------

// 네트워크 복제 시 이름을 UI에 설정 (RepNotify)
void APlayerCharacter::OnRep_PlayerName()
{
	if ( netPlayerUI )
	{
		UUserWidget* widget = netPlayerUI->GetUserWidgetObject();
		if ( UNet_PlayerUI* playerUI = Cast<UNet_PlayerUI>(widget) )
		{
			playerUI->SetPlayerName(playerName);  // UI에 playerName을 설정
		}
	}
}

//--------------------------------------------------------------------------------
// 채팅
void APlayerCharacter::ServerRPC_SendMsg_Implementation(const FString& msg)
{
	MultiRPC_SendMsg(msg);
}

void APlayerCharacter::MultiRPC_SendMsg_Implementation(const FString& msg)
{
	auto pc = Cast<ANetPlayerController>(GetWorld()->GetFirstPlayerController());

	if ( pc ) {
		pc->infoUI->ReceiveMsg(msg);
	}
}

//--------------------------------------------------------------------------------
// 단해가 만듦
// 보스 스킬 3 공격을 맞을 경우
void APlayerCharacter::ApplyTornadoDamagetoPlayer(int32 damage)
{
	if ( HitDelayTime <= 0.0f )
	{
		// 클라이언트일 경우 서버에 데미지 적용 요청
		if ( !HasAuthority() )
		{
			Server_ApplyDamage(damage);
			return;
		}

		// 서버일 경우, 실제 데미지 처리 + 연출 전파
		int32 playerNewHP = GetHP() - damage;
		SetHP(playerNewHP);

		// 연출 전파
		Multicast_PlayDamageEffect();

		HitDelayTime = 0.89f;
	}
}

//--------------------------------------------------------------------------------
void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 여기에서 동기화할 변수들 등록
	DOREPLIFETIME(APlayerCharacter , hp);
	DOREPLIFETIME(APlayerCharacter , WeaponComp);
	DOREPLIFETIME(APlayerCharacter , playerName);
	DOREPLIFETIME(APlayerCharacter , Playerclass);


}







