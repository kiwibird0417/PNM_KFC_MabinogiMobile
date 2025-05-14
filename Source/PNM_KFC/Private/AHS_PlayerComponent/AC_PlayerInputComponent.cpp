// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_PlayerComponent/AC_PlayerInputComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "AHS_UI/PlayerMainUI.h"
#include "AHS_PlayerComponent/AC_PlayerSkillComponent.h"
#include "AHS_UI/PlayerInfo/PlayerInfoDetailsUI.h"
#include "Blueprint/UserWidget.h"
#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"
#include "AHS_PlayerComponent/AC_PlayerInventoryComponent.h"
#include "AHS_UI/PlayerInfo/PlayerInventoryUI.h"
#include "Components/Image.h"
#include "AHS_UI/PlayerSkillUI.h"
#include "GameFramework/PlayerController.h"
#include "AHS_Network/NetGameInstance.h"


UAC_PlayerInputComponent::UAC_PlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	// 1. 컨트롤러(IMC) 연결
	ConstructorHelpers::FObjectFinder<UInputMappingContext>TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/AHS/Inputs/IMC_LocalPlayer.IMC_LocalPlayer'"));

	if (TempIMC.Succeeded()) {
		IMC_LocalPlayer = TempIMC.Object;
	}

	//-----------------------------------------------------------------------
	// 1-1. 디버깅용 테스트 Action
	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_Debug(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_DebugTest.IA_DebugTest'"));
	if ( TempIA_Debug.Succeeded() ) {
		IA_DebugTest = TempIA_Debug.Object;
	}


	//-----------------------------------------------------------------------
	// 2. 이동 Input Action
	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_Move(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_PlayerMove.IA_PlayerMove'"));
	if (TempIA_Move.Succeeded()) {
		IA_PlayerMove = TempIA_Move.Object;
	}

	//-----------------------------------------------------------------------
	// 3. 카메라 관련 Action
	// 3-1. 카메라 회전
	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_CameraRotate(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_CameraRotation.IA_CameraRotation'"));
	if (TempIA_CameraRotate.Succeeded()) {
		IA_CameraRotation = TempIA_CameraRotate.Object;
	}

	// 3-2. 카메라 줌인/줌아웃
	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_CameraZoom(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_CameraZoom.IA_CameraZoom'"));
	if (TempIA_CameraZoom.Succeeded()) {
		IA_CameraZoom = TempIA_CameraZoom.Object;
	}

	//-----------------------------------------------------------------------
	// 4. 상호작용
	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_Interaction(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_PlayerInteraction.IA_PlayerInteraction'"));
	if (TempIA_Interaction.Succeeded()) {
		IA_InterAction = TempIA_Interaction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_LeftMouseClick(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_LeftMouseClick.IA_LeftMouseClick'"));
	if (TempIA_LeftMouseClick.Succeeded()) {
		IA_LeftMouseClick = TempIA_LeftMouseClick.Object;
	}

	//-----------------------------------------------------------------------
	// 5. 스킬 Input Action 연결

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill01(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_PlayerSkill01.IA_PlayerSkill01'"));
	if ( TempIA_Skill01.Succeeded() ) {
		IA_PlayerSkill01 = TempIA_Skill01.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill02(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_PlayerSkill02.IA_PlayerSkill02'"));
	if ( TempIA_Skill02.Succeeded() ) {
		IA_PlayerSkill02 = TempIA_Skill02.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill03(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_PlayerSkill03.IA_PlayerSkill03'"));
	if ( TempIA_Skill03.Succeeded() ) {
		IA_PlayerSkill03 = TempIA_Skill03.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill04(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_PlayerSkill04.IA_PlayerSkill04'"));
	if ( TempIA_Skill04.Succeeded() ) {
		IA_PlayerSkill04 = TempIA_Skill04.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill05(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_PlayerSkill05.IA_PlayerSkill05'"));
	if ( TempIA_Skill05.Succeeded() ) {
		IA_PlayerSkill05 = TempIA_Skill05.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill06(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_PlayerSkill06.IA_PlayerSkill06'"));
	if ( TempIA_Skill06.Succeeded() ) {
		IA_PlayerSkill06 = TempIA_Skill06.Object;
	}

	//-----------------------------------------------------------------------
	// 6. 플레이어 정보, 가방(인벤토리) 연결
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_PlayerInfo(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_PlayerInfo.IA_PlayerInfo'"));
	if ( TempIA_PlayerInfo.Succeeded() ) {
		IA_PlayerInfo = TempIA_PlayerInfo.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Inventory(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_Inventory.IA_Inventory'"));
	if ( TempIA_Inventory.Succeeded() ) {
		IA_Inventory = TempIA_Inventory.Object;
	}


	//-----------------------------------------------------------------------
	// 7. 퀵슬롯 등록(임시 기능으로 매핑)
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_QuickSlot07(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_QuickSlot07.IA_QuickSlot07'"));
	if ( TempIA_QuickSlot07.Succeeded() ) {
		IA_QuickSlot07 = TempIA_QuickSlot07.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_QuickSlot08(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_QuickSlot08.IA_QuickSlot08'"));
	if ( TempIA_QuickSlot08.Succeeded() ) {
		IA_QuickSlot08 = TempIA_QuickSlot08.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_QuickSlot09(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions_Battle/IA_QuickSlot09.IA_QuickSlot09'"));
	if ( TempIA_QuickSlot09.Succeeded() ) {
		IA_QuickSlot09 = TempIA_QuickSlot09.Object;
	}



	//-----------------------------------------------------------------------
	// [*] 플레이어 인벤토리, 무기 관련 정보 디버깅용 테스트
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_DebugTest1(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_DebugTest1.IA_DebugTest1'"));
	if ( TempIA_DebugTest1.Succeeded() ) {
		IA_DebugTest1 = TempIA_DebugTest1.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_DebugTest2(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_DebugTest2.IA_DebugTest2'"));
	if ( TempIA_DebugTest2.Succeeded() ) {
		IA_DebugTest2 = TempIA_DebugTest2.Object;
	}


	//-----------------------------------------------------------------------
	// 게임 실행 중 마우스를 보이게 하고, 다른 액션 없이 할 수 있도록
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_ShowMouseCursorAlt(TEXT("/Script/EnhancedInput.InputAction'/Game/AHS/Inputs/InputActions/IA_ShowMouseAlt.IA_ShowMouseAlt'"));
	if ( TempIA_ShowMouseCursorAlt.Succeeded() ) {
		IA_ShowMouseCursorAlt = TempIA_ShowMouseCursorAlt.Object;
	}




}


// Begin Play
void UAC_PlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// 플레이어 컨트롤러 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());

	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PlayerController) {
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer) {
			UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (Subsystem && IMC_LocalPlayer) {
				Subsystem->AddMappingContext(IMC_LocalPlayer, 0);
			}
		}
	}

}


// Tick
void UAC_PlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// 입력
void UAC_PlayerInputComponent::SetupInputComponent(APlayerController* PlayerController, UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// 0. 디버깅용 테스트
		EnhancedInputComponent->BindAction(IA_DebugTest , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::DebugTest);


		//-----------------------------------------------------------------
		// 1. 이동
		EnhancedInputComponent->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, this, &UAC_PlayerInputComponent::Move);

		// 2. 카메라 (회전, 줌인/줌아웃)
		EnhancedInputComponent->BindAction(IA_CameraRotation, ETriggerEvent::Triggered, this, &UAC_PlayerInputComponent::Look);
		EnhancedInputComponent->BindAction(IA_CameraZoom, ETriggerEvent::Triggered, this, &UAC_PlayerInputComponent::Zoom);

		// 3. 플레이어 상호작용
		EnhancedInputComponent->BindAction(IA_LeftMouseClick, ETriggerEvent::Started, this, &UAC_PlayerInputComponent::LeftMouseClick);
		EnhancedInputComponent->BindAction(IA_InterAction, ETriggerEvent::Started, this, &UAC_PlayerInputComponent::InterAction);

		//-----------------------------------------------------------------
		// 4. 플레이어 스킬 사용(공격 모드시)
		EnhancedInputComponent->BindAction(IA_PlayerSkill01 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::UseSkill01);
		EnhancedInputComponent->BindAction(IA_PlayerSkill02 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::UseSkill02);
		EnhancedInputComponent->BindAction(IA_PlayerSkill03 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::UseSkill03);
		EnhancedInputComponent->BindAction(IA_PlayerSkill04 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::UseSkill04);
		EnhancedInputComponent->BindAction(IA_PlayerSkill05 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::UseSkill05);
		EnhancedInputComponent->BindAction(IA_PlayerSkill06 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::UseSkill06);


		//-----------------------------------------------------------------
		// 5. 플레이어 메뉴 UI
		EnhancedInputComponent->BindAction(IA_PlayerInfo , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::OpenPlayerInfoMenu);

		// 플레이어 인베노리 메뉴 UI
		EnhancedInputComponent->BindAction(IA_Inventory , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::OpenPlayerInventoryMenu);

		//-----------------------------------------------------------------
		// 6. 무기 장착 관련 Debuging Test용
		EnhancedInputComponent->BindAction(IA_DebugTest1 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::DebugTest_AddWeapon);
		EnhancedInputComponent->BindAction(IA_DebugTest2 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::DebugTest_PrintInventoryWeapon);

		//-----------------------------------------------------------------
		// 7. 퀵슬롯 관련 액션
		EnhancedInputComponent->BindAction(IA_QuickSlot07 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::QuickSlot07);
		EnhancedInputComponent->BindAction(IA_QuickSlot08 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::QuickSlot08);
		EnhancedInputComponent->BindAction(IA_QuickSlot09 , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::QuickSlot09);

		//-----------------------------------------------------------------
		// 8. 마우스 커서 보이기( + alt)
		EnhancedInputComponent->BindAction(IA_ShowMouseCursorAlt , ETriggerEvent::Started , this , &UAC_PlayerInputComponent::ShowMouseCursorAlt);
		EnhancedInputComponent->BindAction(IA_ShowMouseCursorAlt , ETriggerEvent::Completed , this , &UAC_PlayerInputComponent::ShowMouseCursorAlt);



	}
}

//===============================================================================
// 디버깅 테스트용 함수
void UAC_PlayerInputComponent::DebugTest()
{
	// 예외 처리
	if ( isShowingDetailUI ) {
		return;
	}

	//-----------------------------------------------------------

	OwnerCharacter->Attack();
}

//===============================================================================
// 이동(기본)
void UAC_PlayerInputComponent::Move(const FInputActionValue& Value)
{

	// 예외 처리
	if ( !OwnerCharacter || isShowingDetailUI || OwnerCharacter->bIsPlayerDead || bShowMouseCursorAlt) {
		return;
	}

	// 이동을 하면, 기본 상태로 돌아가야 함(스킬을 사용하고 있다면, 공격 상태로)
	if ( OwnerCharacter->bUsingSkill == true || OwnerCharacter->mState == EPlayerState::Skill) {
		OwnerCharacter->bUsingSkill = false;
		OwnerCharacter->bSkillEnded = true;
		OwnerCharacter->mState = EPlayerState::Basic;
	}


	//---------------------------------------------------------------

	FVector2D MovementVector = Value.Get<FVector2D>();

	// 자동모드를 비활성화한다.
	OwnerCharacter->SetAutoState(false);

	bool bIsAuto = OwnerCharacter->bIsAuto;

	// 버튼 이미지 갱신(UI)
	if ( OwnerCharacter->mainUI )
	{
		OwnerCharacter->mainUI->ChangeInterActionButtonImage(bIsAuto);
	}

	if ( OwnerCharacter->mState == EPlayerState::Attack ) {
		OwnerCharacter->mState = EPlayerState::Basic;
	}

	//---------------------------------------------------------------
	// 컨트롤러의 회전값을 가져옴 (카메라가 바라보는 방향)
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	ControlRotation.Pitch = 0.0f; // 상하 회전 무시 (이동에 필요 없음)
	ControlRotation.Roll = 0.0f;

	// 캐릭터가 바라보는 방향을 기준으로 이동 벡터 변환
	FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	// 입력 값을 방향 벡터와 결합하여 이동 방향 결정
	FVector MoveDirection = ( ForwardDirection * MovementVector.Y ) + ( RightDirection * MovementVector.X );

	OwnerCharacter->AddMovementInput(MoveDirection);
}



//===============================================================================
// 플레이어 카메라 회전
void UAC_PlayerInputComponent::Look(const FInputActionValue& Value)
{
	// 예외 처리
	if ( isShowingDetailUI || isShowingInventoryUI || OwnerCharacter->bIsPlayerDead || bShowMouseCursorAlt) {
		return;
	}

	//-----------------------------------------------------------
	if ( OwnerCharacter->IsLocallyControlled() ) {
		FVector2D value = Value.Get<FVector2D>();


		OwnerCharacter->AddControllerYawInput(value.X);
		OwnerCharacter->AddControllerPitchInput(-value.Y);
	}


	
}


// 플레이어 줌인/줌아웃
void UAC_PlayerInputComponent::Zoom(const FInputActionValue& Value)
{
	// 예외 처리
	if ( isShowingDetailUI || OwnerCharacter->bIsPlayerDead) {
		return;
	}


	//-----------------------------------------------------------
	float ZoomValue = Value.Get<float>(); // 마우스 휠 값 (양수 = 줌인, 음수 = 줌아웃)

	if (!OwnerCharacter) return;

	// 캐릭터의 SpringArm 가져오기
	USpringArmComponent* CameraBoom = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
	if (!CameraBoom) return;

	// 줌 조정 값 설정
	float ZoomSpeed = 50.0f;  // 줌 속도 조절
	float MinZoom = 100.0f;    // 최소 거리 (최대로 가까운 값)
	float MaxZoom = 500.0f;    // 최대 거리 (최대로 멀리 보이는 값)

	// 현재 Boom 길이에 Zoom 적용
	float NewTargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - (ZoomValue * ZoomSpeed), MinZoom, MaxZoom);

	// SpringArm 업데이트
	CameraBoom->TargetArmLength = NewTargetArmLength;
}

//===============================================================================
// 왼쪽 마우스 버튼 클릭 (상호작용 취소)
void UAC_PlayerInputComponent::LeftMouseClick(const FInputActionValue& Value)
{
	// 예외 처리
	if ( isShowingDetailUI || OwnerCharacter->bIsPlayerDead) {
		return;
	}

	//-----------------------------------------------------------


		// 자동모드를 비활성화한다.
		OwnerCharacter->SetAutoState(false);

		bool bIsAuto = OwnerCharacter->bIsAuto;


		if ( OwnerCharacter->mainUI )
		{
			// 버튼 이미지 갱신(UI)
			OwnerCharacter->mainUI->ChangeInterActionButtonImage(bIsAuto);
		}
		else
		{
			UE_LOG(LogTemp , Warning , TEXT("mainUI is NULL in LeftMouseClick"));
		}


		if ( OwnerCharacter->mState == EPlayerState::Attack ) {
			OwnerCharacter->mState = EPlayerState::Basic;
		}
	
}

//===============================================================================
// 플레이어 상호작용 : 스페이스 바
void UAC_PlayerInputComponent::InterAction(const FInputActionValue& Value)
{
	// 예외 처리
	if ( isShowingDetailUI || OwnerCharacter->bIsPlayerDead) {
		return;
	}

	// 일반 상태로 돌아오기 (스킬 상태에서...)
	if ( OwnerCharacter->bUsingSkill == true ) {
		OwnerCharacter->bUsingSkill = false;
		OwnerCharacter->bSkillEnded = true;
		OwnerCharacter->mState = EPlayerState::Basic;
	}

	//-----------------------------------------------------------


		// 자동모드 활성화 & 비활성화
		OwnerCharacter->SetAuto();

		// 플레이어 auto mode 들고 오기
		bool bIsAuto = OwnerCharacter->bIsAuto;

		// UI 갱신(버튼 이미지)
		if ( OwnerCharacter->mainUI )
		{
			OwnerCharacter->mainUI->ChangeInterActionButtonImage(bIsAuto);
		}

		
		if ( OwnerCharacter->mState == EPlayerState::Attack ) {
			OwnerCharacter->mState = EPlayerState::Basic;
		}
		

		//----------------------------------------------------------------------
		// 공격 가능한 상태라면
		if ( OwnerCharacter->canAttack )
		{
			OwnerCharacter->isTrackingTarget = true;
		}


	
}

//===============================================================================
// 스킬 사용
void UAC_PlayerInputComponent::UseSkill01()
{
	// 스킬 사용중이라면, 중복으로 스킬을 사용할 수 없도록 예외 처리
	if ( OwnerCharacter->bUsingSkill == true || OwnerCharacter->bIsPlayerDead) {
		return;
	}

	//----------------------------------------------
	OwnerCharacter->bUsingSkill = true;
	//-----------------------------------------------

	OwnerCharacter->PlayerSkillComp->UseSkill(0);

	OwnerCharacter->skillUI->StartCooldown();
}

void UAC_PlayerInputComponent::UseSkill02()
{
	// 스킬 사용중이라면, 중복으로 스킬을 사용할 수 없도록 예외 처리
	if ( OwnerCharacter->bUsingSkill || OwnerCharacter->bIsPlayerDead ) {
		return;
	}
	//----------------------------------------------
	OwnerCharacter->bUsingSkill = true;

	OwnerCharacter->PlayerSkillComp->UseSkill(1);
	OwnerCharacter->skillUI->StartCooldown02();
}

void UAC_PlayerInputComponent::UseSkill03()
{
	// 스킬 사용중이라면, 중복으로 스킬을 사용할 수 없도록 예외 처리
	if ( OwnerCharacter->bUsingSkill || OwnerCharacter->bIsPlayerDead ) {
		return;
	}

	OwnerCharacter->PlayerSkillComp->UseSkill(2);
}

void UAC_PlayerInputComponent::UseSkill04()
{
	// 스킬 사용중이라면, 중복으로 스킬을 사용할 수 없도록 예외 처리
	if ( OwnerCharacter->bUsingSkill || OwnerCharacter->bIsPlayerDead ) {
		return;
	}

	OwnerCharacter->PlayerSkillComp->UseSkill(3);
}

void UAC_PlayerInputComponent::UseSkill05()
{
	// 스킬 사용중이라면, 중복으로 스킬을 사용할 수 없도록 예외 처리
	if ( OwnerCharacter->bUsingSkill || OwnerCharacter->bIsPlayerDead ) {
		return;
	}

	OwnerCharacter->PlayerSkillComp->UseSkill(4);
}

void UAC_PlayerInputComponent::UseSkill06()
{
	// 스킬 사용중이라면, 중복으로 스킬을 사용할 수 없도록 예외 처리
	if ( OwnerCharacter->bUsingSkill || OwnerCharacter->bIsPlayerDead ) {
		return;
	}

	OwnerCharacter->PlayerSkillComp->UseSkill(5);
}

//===============================================================================
// 플레이어 Info UI 메뉴
void UAC_PlayerInputComponent::OpenPlayerInfoMenu(const FInputActionValue& Value)
{
	ShowAndHideDetailUI();	
}

void UAC_PlayerInputComponent::ShowAndHideDetailUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if ( !isShowingDetailUI )
	{
		if ( detailsUIWidget )
		{
			detailsUI = Cast<UPlayerInfoDetailsUI>(CreateWidget(GetWorld() , detailsUIWidget));

			if ( detailsUI )
			{
				detailsUI->AddToViewport();

				// 마우스 커서만 보이게
				if ( PC )
				{
					PC->bShowMouseCursor = true;
				}
			}
		}

		isShowingDetailUI = true;
	}
	else
	{
		if ( detailsUI )
		{
			detailsUI->RemoveFromParent();
			detailsUI = nullptr;

			// 마우스 커서 숨기기
			if ( PC )
			{
				PC->bShowMouseCursor = false;
			}
		}

		isShowingDetailUI = false;
	}
}

void UAC_PlayerInputComponent::CloseInfoMenu()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if ( detailsUI )
	{
		detailsUI->RemoveFromParent();
		detailsUI = nullptr;

		// 마우스 커서 숨기기
		if ( PC )
		{
			PC->bShowMouseCursor = false;
		}
	}

	isShowingDetailUI = false;
}

//===============================================================================
// 플레이어 인벤토리 UI 메뉴
void UAC_PlayerInputComponent::OpenPlayerInventoryMenu(const FInputActionValue& Value)
{
	ShowAndHideInventoryUI();
}

void UAC_PlayerInputComponent::ShowAndHideInventoryUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if ( !isShowingInventoryUI ) {
		if ( inventoryUIWidget )
		{
			inventoryUI = Cast<UPlayerInventoryUI>(CreateWidget(GetWorld() , inventoryUIWidget));

			if ( inventoryUI )
			{
				inventoryUI->AddToViewport();

				UAC_PlayerInventoryComponent* InventoryComponent = OwnerCharacter->PlayerInventoryComp;
				if ( InventoryComponent )
				{
					const TArray<UWeaponInstance*>& WeaponList = InventoryComponent->GetAllWeapons();

					//=========================================================
					inventoryUI->PopulateWeaponInventory(WeaponList);
				}

				// 현재 장착 무기 이미지 세팅
				if ( APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OwnerCharacter) )
				{
					if ( PlayerChar->GetEquippedWeapon() ) // 또는 EquippedWeapon 접근자
					{
						UWeaponInstance* EquippedWeapon = PlayerChar->GetEquippedWeapon();
						if ( EquippedWeapon && inventoryUI->EquippedWeaponImage )
						{
							UTexture2D* WeaponImage = EquippedWeapon->GetWeaponImage();
							if ( WeaponImage )
							{
								inventoryUI->EquippedWeaponImage->SetBrushFromTexture(WeaponImage);
							}
						}
					}
				}

				if ( PC )
				{
					PC->bShowMouseCursor = true;
				}
			}
		}

		isShowingInventoryUI = true;
	}
	else {
		if ( inventoryUI )
		{
			inventoryUI->RemoveFromParent();
			inventoryUI = nullptr;

			// 마우스 커서 숨기기
			if ( PC )
			{
				PC->bShowMouseCursor = false;
			}
		}

		isShowingInventoryUI = false;
	}
}

void UAC_PlayerInputComponent::CloseInventoryMenu()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if ( inventoryUI )
	{
		inventoryUI->RemoveFromParent();
		inventoryUI = nullptr;

		// 마우스 커서 숨기기
		if ( PC )
		{
			PC->bShowMouseCursor = false;
		}
	}

	isShowingInventoryUI = false;
}



//===============================================================================
// 2. 무기 장착을 위한 디버깅용 테스트 함수
// 무기를 더함
// 단일 무기 추가
/*
void UAC_PlayerInputComponent::DebugTest_AddWeapon(const FInputActionValue& Value)
{
	if ( AActor* Owner = GetOwner() )
	{
		UAC_PlayerInventoryComponent* InventoryComp = Owner->FindComponentByClass<UAC_PlayerInventoryComponent>();
		if ( !InventoryComp || !InventoryComp->WeaponDataTable ) return;

		// 1. 사용할 무기 RowName (DataTable의 Row Name과 정확히 일치해야 함)
		FName WeaponID = TEXT("1");

		// 2. RowName으로 데이터 테이블에서 무기 정보 가져오기
		const FWeaponData* RowData = InventoryComp->WeaponDataTable->FindRow<FWeaponData>(WeaponID , TEXT("Lookup WeaponData"));

		if ( RowData )
		{
			UWeaponInstance* NewWeapon = NewObject<UWeaponInstance>(InventoryComp);
			NewWeapon->InitializeInstance(WeaponID , *RowData);

			InventoryComp->AddWeaponInstance(NewWeapon);

			UE_LOG(LogTemp , Log , TEXT("[Debug] 무기를 인벤토리에 추가했습니다: %s") , *NewWeapon->GetWeaponName().ToString());
		}
		else
		{
			UE_LOG(LogTemp , Error , TEXT("무기 데이터 테이블에서 %s 을(를) 찾을 수 없습니다.") , *WeaponID.ToString());
		}

	}
}
*/

// 여러 개의 무기 추가 --> 플레이어에서 초기화 시 추가중!
void UAC_PlayerInputComponent::DebugTest_AddWeapon(const FInputActionValue& Value)
{
	// 임시로 플레이어 데미지 로직 연결
	OwnerCharacter->ApplyDamagetoPlayer(500);

}



// 전체 무기 출력
void UAC_PlayerInputComponent::DebugTest_PrintInventoryWeapon(const FInputActionValue& Value)
{
	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if ( gi )
	{
		// 게임 인스턴스에서 세션 이름을 받아오기 (예시: 플레이어가 입력한 이름)
		FString inputName = *gi->mySessionName;

		// 서버에 이름을 전송
		if ( OwnerCharacter->HasAuthority() )
		{
			OwnerCharacter->playerName = inputName;
		}
		else
		{
			OwnerCharacter->ServerRPC_SetPlayerName(inputName);
		}

		OwnerCharacter->OnRep_PlayerName();
	}
}

//===============================================================================
// 퀵 슬롯 임시 구현(나중에 아이템 인벤토리와 연동시키기)
void UAC_PlayerInputComponent::QuickSlot07(const FInputActionValue& Value)
{
	// 플레이어 HP 회복
	OwnerCharacter->SetHP(OwnerCharacter->MaxHP);
}

void UAC_PlayerInputComponent::QuickSlot08(const FInputActionValue& Value)
{
	// 플레이어 상태 회복
}

void UAC_PlayerInputComponent::QuickSlot09(const FInputActionValue& Value)
{
	// 플레이어 부활
}

//===============================================================================
// UI 조작을 위해, 마우스 커서 보이도록 설정
void UAC_PlayerInputComponent::ShowMouseCursorAlt(const FInputActionValue& Value)
{
	auto* pc = GetWorld()->GetFirstPlayerController();

	if ( bShowMouseCursorAlt ) {
		bShowMouseCursorAlt = false;
		
		if ( pc ) {
			pc->bShowMouseCursor = false;
			pc->SetInputMode(FInputModeGameOnly());
		}
	}
	else {
		bShowMouseCursorAlt = true;


		if ( pc ) {
			pc->bShowMouseCursor = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);

			pc->SetInputMode(InputMode);
		}
	}
}

//===============================================================================