#include "ODH_Boss/BossFSM.h"
#include "ODH_EnemyData/EnemyDataTables.h"
#include "Kismet/GameplayStatics.h"
#include "ODH_Boss/BossEnemy.h"
#include "ODH_EnemyAnim/BossAnim.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ODH_EnemyUI/EnemyUI.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "AHS_Network/NetPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LevelScriptActor.h"


UBossFSM::UBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABossEnemy>(GetOwner());
	if ( me )
	{
		Anim = Cast<UBossAnim>(me->GetMesh()->GetAnimInstance());
		/*EnemyHPUI = Cast<UEnemyUI>(me->UIComp->GetWidget());*/
	}

	if ( me->HasAuthority() )
	{
		OnRep_RandomTargetChange();
	}
}

void UBossFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBossFSM, CurHP);
	DOREPLIFETIME(UBossFSM , MaxHP);
	DOREPLIFETIME(UBossFSM, Target);
}

void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(7 , 1.0f , FColor::Yellow , logMsg);

	if ( !EnemyHPUI )
	{
		EnemyHPUI = Cast<UEnemyUI>(me->UIComp->GetWidget());
	}

	if ( bIsDie )
	{
		CurDieTime+=DeltaTime;

		if ( CurDieTime >= DelayDieTime )
		{
			me->Destroy();
		}
	}

	if ( CurHP <= 0 && mState != EBossState::DIE && !bIsDie )
	{
		//다시 안불리게 불 전환
		bIsDie = true;
		//사망 상태 전환
		mState = EBossState::DIE;
		//각 플레이어들 못 움직이기고 UI 나오도록 함
		ServerRPC_ShowClearWidget();

		//클리어 BGM 나오게 만듦
		ChangeBGM(true);
	}

	if ( bEaseFunctionMove )
	{
		FunctionTimer += GetWorld()->GetDeltaSeconds();
		Alpha = FMath::Clamp(FunctionTimer / FunctionDuration , 0.0f , 1.0f);

		float EaseAlpha = EaseOutExpo(Alpha);

		FVector StartLocation = me->GetActorLocation();

		//내가 봤을 땐 계속해서 타겟의 위치가 변해서 그러는 듯
		FVector NewLocation = FMath::Lerp(StartLocation ,EasingLocation , EaseAlpha);

		if ( me )
		{
			me->SetActorLocation(NewLocation);
		}

		if ( Alpha >= 1.0f )
		{
			bEaseFunctionMove = false;
			FunctionTimer = 0.0f;
		}
	}

	switch ( mState )
	{
		case EBossState::IDLE: { IDLEState(); } break;
		case EBossState::CHASE: { CHASEState(); } break;
		case EBossState::ATTACK: { ATTACKState(); } break;
		case EBossState::RANGEDATTACK: { RANGEDATTACKState(); } break;
		case EBossState::DAMAGE: { DAMAGEState(); } break;
		case EBossState::BREAK: { BREAKState(); } break;
		case EBossState::DIE: { DIEState(); } break;
	}
}

void UBossFSM::LoadStat(FName RowName)
{
	if ( EnemyDataTable )
	{
		FEnemyDataTable* EnemyStat = EnemyDataTable-> FindRow<FEnemyDataTable>(RowName, TEXT(""));
		if ( EnemyStat )
		{
			MaxHP = EnemyStat->MaxHP;
			CurHP = MaxHP;
			BreakHP = EnemyStat->BreakHP;
			Speed = EnemyStat->Speed;
			EXP = EnemyStat->EXP;
			Gold = EnemyStat->Gold;
		}
	}
}

void UBossFSM::SetHP(float value)
{
	CurHP = value;	//이거는 남겨야하고

	OnRep_HP();
}

float UBossFSM::GetHP()
{
	return CurHP;
}

void UBossFSM::OnRep_HP()
{
	float percent = CurHP / MaxHP;

	if ( EnemyHPUI )
	{
		EnemyHPUI->hp = percent;
	}
}

void UBossFSM::ServerRPC_ShowClearWidget_Implementation()
{
	if ( me->HasAuthority() )
	{
		MultiCastRPC_ShowClearWidget();
	}
}

void UBossFSM::MultiCastRPC_ShowClearWidget_Implementation()
{
	for ( FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It )
	{
		if ( ANetPlayerController* pc = Cast<ANetPlayerController>(*It) )
		{	
			//pc에서 UI가 보이도록 만들어줘야 함
			pc->ClientRPC_ShowClearUI();

			if ( APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()) )
			{
				player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}
		}
	}
}

void UBossFSM::ChangeBGM(bool bClear)
{
	ALevelScriptActor* levelScript = GetWorld()->GetLevelScriptActor();

	if ( levelScript )
	{
		FProperty* boolProperty = levelScript->GetClass()->FindPropertyByName(TEXT("bClear"));
		if ( boolProperty )
		{
			bool* boolVaule = boolProperty->ContainerPtrToValuePtr<bool>(levelScript);
			if ( boolVaule )
			{
				*boolVaule = bClear;
			}
		}
	}
}

void UBossFSM::SetTarget(APlayerCharacter* player)
{
	Target = player;
}

void UBossFSM::StartAttackPlayer()
{
	//거리가 얼마나 벌어져 있든 공격 애니메이션이 실행되면 타겟을 공격
	Target->ApplyDamagetoPlayer(10);

	me->AttackParticle->SetActive(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwingSound, me->GetActorLocation());
}

void UBossFSM::EndAttackPlayer()
{
	if ( me->HasAuthority() )
	{
		ServerRPC_SearchPlayer(this);
	}

	//끝나면 RanNum을 0으로 초기화
	RanNum = 0;

	//끝났다는 알림이 오면 State를 변경해줌
	mState = EBossState::CHASE;
	Anim->AnimState = mState;
	
	ChaseTime = 0.0f;

	FunctionTimer = 0.0f;
}

void UBossFSM::StartSwingAttack()
{
	if ( me->HasAuthority() )
	{
		ServerRPC_SwingCheck();
	}

	me->SwingParticle->SetActive(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld() , SwingSound , me->GetActorLocation());
}

void UBossFSM::ServerRPC_SwingCheck_Implementation()
{
	UWorld* World = me->GetWorld();
	if ( !World ) return;

	FVector TraceStart = me->GetActorLocation();
	FVector TraceExtend = FVector(250/*400*/ , 250/*400*/ , /*100*/200);
	FRotator TraceRot = me->GetActorRotation();

	TArray<FHitResult> HitResults;
	FCollisionShape Box = FCollisionShape::MakeBox(TraceExtend);

	bool bHit = World->SweepMultiByChannel(
		HitResults ,
		TraceStart ,
		TraceStart ,
		TraceRot.Quaternion() ,
		ECC_GameTraceChannel2 ,
		Box
	);

	//DrawDebugBox(World , TraceStart , TraceExtend , TraceRot.Quaternion() , FColor::Red , false , 5.0f);

	bool bPlayerHit = false;

	if ( bHit )
	{
		TArray<APlayerCharacter*> players;
		for ( const FHitResult& Hit : HitResults )
		{
			AActor* HitActor = Hit.GetActor();

			if ( players.Num() == 0 )
			{
				if ( HitActor && HitActor != me && HitActor->IsA(APlayerCharacter::StaticClass()) )
				{
					APlayerCharacter* hitPlayer = Cast<APlayerCharacter>(Hit.GetActor());

					MulticastRPC_SwingCheck(hitPlayer);
					//hitPlayer->ApplyDamagetoPlayer(9);

					players.Add(hitPlayer);
				}
			}

			else
			{
				for ( int32 i = 0; i < players.Num(); ++i )
				{
					if ( HitActor && HitActor != me && HitActor->IsA(APlayerCharacter::StaticClass()) && players[i] != Cast<APlayerCharacter>(Hit.GetActor()) )
					{
						APlayerCharacter* hitPlayer = Cast<APlayerCharacter>(Hit.GetActor());

						MulticastRPC_SwingCheck(hitPlayer);
						//hitPlayer->ApplyDamagetoPlayer(9);

						players.Add(hitPlayer);
					}
				}
			}
		}
	}
}

void UBossFSM::MulticastRPC_SwingCheck_Implementation(APlayerCharacter* player)
{
	player->ApplyDamagetoPlayer(9);
}

void UBossFSM::EndSwingAttack()
{
	
}

void UBossFSM::GoToPlayer()
{
	bEaseFunctionMove = true;
	EasingLocation = Target->GetActorLocation();
	/*GetWorld()->SpawnActor<AActor>(AttackRangeEffect, FVector(EasingLocation.X, EasingLocation.Y, EasingLocation.Z -  EasingLocation.Z + 0.4f),FRotator(0));*/

// 	Effect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , MovingEffect, me->GetActorLocation());
// 	Effect->AttachToComponent(me->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
}

void UBossFSM::StartStompAttack()
{
	if ( me->HasAuthority() )
	{
		ServerRPC_StompCheak();
	}

	Alpha = 0.0f;
	bEaseFunctionMove= false;

// 	Effect->DeactivateSystem();
// 	Effect->DestroyComponent();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , StompEffect , me->StompEffecPos->GetComponentLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld() , SwingSound , me->GetActorLocation());
}

void UBossFSM::ServerRPC_StompCheak_Implementation()
{
	UWorld* World = me->GetWorld();
	if ( !World ) return;

	FVector TraceStart = me->GetMesh()->GetSocketLocation(FName("StompLocation"));
	float TraceRadius = 200.f;

	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

	bool bHit = World->SweepMultiByChannel(
		HitResults ,
		TraceStart ,
		TraceStart ,
		FQuat::Identity ,
		ECC_GameTraceChannel2 ,
		Sphere
	);

	//DrawDebugSphere(World , TraceStart , TraceRadius , 16 , FColor::Red , false , 1.0f);

	bool bPlayerHit = false;

	if ( bHit )
	{
		TArray<APlayerCharacter*> players;
		for ( const FHitResult& Hit : HitResults )
		{
			AActor* HitActor = Hit.GetActor();

			if ( players.Num() == 0 )
			{
				if ( HitActor && HitActor != me && HitActor->IsA(APlayerCharacter::StaticClass()) )
				{
					APlayerCharacter* hitPlayer = Cast<APlayerCharacter>(Hit.GetActor());

					MulticastRPC_StompCheak(hitPlayer);

					players.Add(hitPlayer);
				}
			}

			else
			{
				for ( int32 i = 0; i < players.Num(); ++i )
				{
					if ( HitActor && HitActor != me && HitActor->IsA(APlayerCharacter::StaticClass()) && players[i] != Cast<APlayerCharacter>(Hit.GetActor()) )
					{
						APlayerCharacter* hitPlayer = Cast<APlayerCharacter>(Hit.GetActor());

						MulticastRPC_StompCheak(hitPlayer);

						players.Add(hitPlayer);
					}
				}
			}
		}
	}
}

void UBossFSM::MulticastRPC_StompCheak_Implementation( APlayerCharacter* player)
{
	player->ApplyDamagetoPlayer(15);
}	

void UBossFSM::Skill3Attack()
{
	//Z축은 땅에 붙은 상태로 플레이어 위치에 스킬 이펙트 생성
	FVector SpawnLocation = FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y, me->ZPosSceneComp->GetComponentLocation().Z);

	GetWorld()->SpawnActor<AActor>(Skill3Effect,SpawnLocation,FRotator(0));
}

void UBossFSM::DieEnemy()
{

}

void UBossFSM::IDLEState()
{
	//아무것도 안하고 가만히 있음

}

void UBossFSM::CHASEState()
{
	//Anim->AnimState = mState;

	ChaseTime += GetWorld()->DeltaTimeSeconds;

	//만약 추적 시간이 일정 시간을 넘을경우
	if ( ChaseTime >= MaxChaseTime )
	{
		//일정 확률로 원거리 공격이 나오게?
		mState = EBossState::RANGEDATTACK;
		Anim->AnimState = mState;
		return;
	}


	//플레이어를 목표 지점으로 잡음
	FVector playerLocation = Target->GetActorLocation();
	//플레이어를 향한 벡터를 얻음
	FVector Dir = ( playerLocation - me->GetActorLocation() ).GetSafeNormal();
	//플레이어를 향해 움직임
	me->SetActorLocation(me->GetActorLocation() + Dir * Speed * GetWorld()->DeltaTimeSeconds);

	//플레이어를 향해 바라보는 방향을 얻음
	FRotator NewRot = Dir.Rotation();
	//위아래 회전은 안되게 막음
	NewRot.Pitch = 0;
	//해당 방향을 향해 회전
	me->SetActorRotation(NewRot);

	//만약 플레이어와 일정 거리 이내일 경우
	if ( FVector::Distance(me->GetActorLocation() , playerLocation) < AttackRange )
	{
		//공격 상태로 전환
		mState = EBossState::ATTACK;
		Anim->AnimState = mState;
	}
}

void UBossFSM::ATTACKState()
{
	if ( me->HasAuthority() )
	{
		ServerRPC_SetRanNum();
	}
}

void UBossFSM::OnRep_RandomTargetChange()
{
	TargetChange = FMath::RandRange(2, 5);
}

void UBossFSM::ServerRPC_SearchPlayer_Implementation(UBossFSM* boss)
{
	++AttackCount;

	//만약에 공격 횟수와 타겟 변경 카운트
	if ( AttackCount == TargetChange )
	{
		TArray<AActor*> players;

		//현재 월드에 있는 APlayerCharacter를 갖고있는 엑터를	찾아봄
		UGameplayStatics::GetAllActorsOfClass(GetWorld() , APlayerCharacter::StaticClass() , players);

		//해당 클래스를 갖고 있는 엑터로 부터 랜덤하게 뽑을 값	을 설정
		int32 randomIndex = FMath::RandRange(0 , players.Num() - 1);

		//해당 플레이어를 목표로 지정
		APlayerCharacter* target = Cast<APlayerCharacter>(players[randomIndex]);

		//AttackCount를 초기화
		AttackCount = 0;

		//TargetChange 값을 다시 랜덤하게 지정
		/*TargetChange = FMath::RandRange(1 , 3);*/
		OnRep_RandomTargetChange();

		MulticastRPC_SearchPlayer(boss , target);
	}
}

void UBossFSM::MulticastRPC_SearchPlayer_Implementation(UBossFSM* boss , APlayerCharacter* player)
{
	boss->Target = player;
}

void UBossFSM::ServerRPC_SetRanNum_Implementation()
{
	if ( RanNum == 0 )
	{
		RanNum = FMath::RandRange(1 , 100);

		MulticastRPC_SetRanNum(RanNum);
	}
}

void UBossFSM::MulticastRPC_SetRanNum_Implementation(const int32 num)
{
	//스킬 1 : 휘둘러치기
	if ( num >= 1 && num <= 30 )
	{
		//해당 스킬 모션을 재생한 다음
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"Skill 1");

		Anim->AttackType = 2;
		Anim->AnimState = mState;
	}
	//스킬 2 : 내려찍기
	else if ( num >= 31 && num <= 60 )
	{
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"Skill 2");

		Anim->AttackType = 3;
		Anim->AnimState = mState;
	}
	// 스킬 3 : 떠돌이의 춤
	else if ( num >= 61 && num <= 80 )
	{
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"Skill 3");

		Anim->AttackType = 4;
		Anim->AnimState = mState;
	}
	//일반 공격
	else
	{
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"Default Attack");

		Anim->AttackType = 1;
		Anim->AnimState = mState;
	}
}

void UBossFSM::RANGEDATTACKState()
{
	if ( me->HasAuthority() )
	{
		ServerRPC_RangedSetRanNum();
	}
}

void UBossFSM::ServerRPC_RangedSetRanNum_Implementation()
{
	if ( RanNum == 0 )
	{
		RanNum = FMath::RandRange(1 , 100);

		MulticastRPC_RangedSetRanNum(RanNum);
	}
}

void UBossFSM::MulticastRPC_RangedSetRanNum_Implementation(const int32 num)
{
	//스킬 2 : 내려찍기
	if ( num >= 1 && num <= 70 )
	{
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"Skill 2");

		Anim->AttackType = 3;
		Anim->AnimState = mState;
	}
	// 스킬 3 : 떠돌이의 춤
	if ( num >= 71 && num <= 100 )
	{
		//GEngine->AddOnScreenDebugMessage(4 , 1.0f , FColor::Green , L"Skill 3");

		Anim->AttackType = 4;
		Anim->AnimState = mState;
	}
}

void UBossFSM::DAMAGEState()
{
	//피격 애니메이션 재생후

	//다시 추적으로 변경
	//mState = EBossState::CHASE;
}

void UBossFSM::BREAKState()
{
	//한동안 무방비 상태에 빠진 후

	//일어서는 애니메이션이 나오면 다시 추적 상태로 이어짐
	//mState = EBossState::CHASE;
}

void UBossFSM::DIEState()
{
	//사망 애니메이션 재생
	Anim->AnimState = mState;
	Anim->Montage_Play(Anim->DeathMontge);
}

float UBossFSM::EaseOutExpo(float alpha)
{
	return 1.0f - FMath::Pow(2.0f, -10.0f * alpha);
}

