#include "ODH_Enemy/DungeonEnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "AHS_Player/PlayerCharacter.h"
#include "ODH_EnemySpawner/DungeonEnemySpawner.h"
#include "ODH_EnemyData/EnemyDataTables.h"
#include "ODH_EnemyAnim/EnemyAnim.h"
#include "Engine/OverlapResult.h"
#include "Components/SphereComponent.h"
#include "ODH_EnemyUI/EnemyUI.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values for this component's properties
UDungeonEnemyFSM::UDungeonEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UDungeonEnemyFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDungeonEnemyFSM ,CurHP);
	DOREPLIFETIME(UDungeonEnemyFSM , MaxHP);
	DOREPLIFETIME(UDungeonEnemyFSM , Target);
	DOREPLIFETIME(UDungeonEnemyFSM, TargetChange);
}

void UDungeonEnemyFSM::LoadStat(FName RowName)
{
	if ( EnemyDataTable )
	{
		FEnemyDataTable* EnemyStat = EnemyDataTable->FindRow<FEnemyDataTable>(RowName , TEXT(""));
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

// Called when the game starts
void UDungeonEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ATestEnemy>(GetOwner());
	
	if ( me )
	{
		Anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
// 		if ( !me->HasAuthority() )
// 		{
// 			HPUI = Cast<UEnemyUI>(me->UIComp->GetWidget());
// 
// 		}
	}

	//타겟 체인지 수를 몇으로 할 지 미리 정함
	/*TargetChange = FMath::RandRange(1,3);*/
	if ( me->HasAuthority() )
	{
		OnRep_RandomTargetChange();
	}
}


// Called every frame
void UDungeonEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(0 , 1.0f , FColor::Red , logMsg);

	if ( !HPUI )
	{
		HPUI = Cast<UEnemyUI>(me->UIComp->GetWidget());
	}

	if ( bIsDie )
	{
		CurDieTime += DeltaTime;
		if ( CurDieTime >= DieDelayTime )
		{
			if ( Spawner )
			{
				Spawner->RemoveEnemyList();

				//me->Destroy();
			}

			me->Destroy();
		}
	}

	if ( CurHP <= 0 && mState != EDungeonEnemyState::DIE )
	{
		//다시 안불리게 막음
		bIsDie = true;
		//사망 상태 전환
		mState = EDungeonEnemyState::DIE;
		
		if ( me->isBargest )
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Barghest_Die, me->GetActorLocation());
		}
		else if ( me->isMimic )
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld() , Mimic_Die , me->GetActorLocation());
		}
		else if ( me->isBug )
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld() , Bug_Die , me->GetActorLocation());
		}
	}

	switch ( mState )
	{
	case EDungeonEnemyState::IDLE: { IDLEState(); } break;
	case EDungeonEnemyState::CHASE: { CAHSEState(); } break;
	case EDungeonEnemyState::ATTACK: { ATTACKState(); } break;
	case EDungeonEnemyState::DAMAGE: { DAMAGEState(); } break;
	case EDungeonEnemyState::BREAK: { BREAKState (); } break;
	case EDungeonEnemyState::DIE: { DIEState(); } break;
	}
}


void UDungeonEnemyFSM::SetHP(float value)
{
	//if( !me->HasAuthority() ) return; //현재 pc가 서버가 아닐 경우 값 변환이 이루어지지 않음
	CurHP = value;	//이거는 남겨야하고

	OnRep_HP();
}

float UDungeonEnemyFSM::GetHP()
{
	return CurHP;
}

void UDungeonEnemyFSM::OnRep_HP()
{
	float percent = CurHP / MaxHP;

	if( HPUI )
	{
		HPUI->hp = percent;
	}
}

float UDungeonEnemyFSM::CheckDist(FVector targetPos , FVector myPos)
{
	return FVector::Dist(myPos , targetPos);
}

void UDungeonEnemyFSM::StartAttackPlayer()
{
	//거리가 얼마나 벌어져 있든 공격 애니메이션이 실행되면 타겟을 공격
	
	//서버 RPC로 이동함
	//Target->ApplyDamagetoPlayer(10);

	if ( me->HasAuthority() )
	{
		ServerRPC_AttackPlayer();
	}
	else
	{
		ServerRPC_AttackPlayer();
	}

	if ( me->isBargest )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld() , Barghest_Attack , me->GetActorLocation());
	}
	else if ( me->isMimic )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld() , Mimic_Attack , me->GetActorLocation());
	}
	else if ( me->isBug )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld() , Bug_Attack , me->GetActorLocation());
	}
}

void UDungeonEnemyFSM::ServerRPC_AttackPlayer_Implementation()
{
	Target->ApplyDamagetoPlayer(10);

	MulticastRPC_AttackPlayer();
}

void UDungeonEnemyFSM::MulticastRPC_AttackPlayer_Implementation()
{
	//모든 플레이어에게 해당 플레이어가 맞았다고 알려줌?
}

void UDungeonEnemyFSM::EndAttackPlayer()
{
	if ( me->HasAuthority() )
	{
		ServerRPC_SearchPlayer(this);
	}

// 	//공격 수 카운트를 1 늘려줌
// 	++AttackCount;
// 
// 	//만약에 공격 횟수와 타겟 변경 카운트 
// 	if ( AttackCount == TargetChange )
// 	{
// 		if ( me->HasAuthority() )
// 		{
// 			ServerRPC_SearchPlayer(this);
// 		}
// 		else
// 		{	
// 			ServerRPC_SearchPlayer(this);
// 		}
// 	}
	
	//끝났다는 알림이 오면 state를 변경해줌
	mState = EDungeonEnemyState::CHASE;
	Anim->AnimState = mState;
}

void UDungeonEnemyFSM::ServerRPC_SearchPlayer_Implementation(UDungeonEnemyFSM* enemy)
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

		Multicast_SearchPlayer(enemy , target);
	}
}

void UDungeonEnemyFSM::Multicast_SearchPlayer_Implementation(UDungeonEnemyFSM* enemy , APlayerCharacter* player)
{
	enemy->Target = player;
}

void UDungeonEnemyFSM::OnRep_RandomTargetChange()
{
	TargetChange = FMath::RandRange(1 , 3);
}

void UDungeonEnemyFSM::DieEnemy()
{
	//스포너에게 자신이 죽었다는 정보를 알려줌
// 	if ( Spawner )
// 	{
// 		Spawner->RemoveEnemyList();
// 
// 		//me->Destroy();
// 	}

	bIsDie = true;
}

void UDungeonEnemyFSM::IDLEState()
{
	//스폰이 되어서 아직 플레이어를 탐지하지 못한 상태
}

void UDungeonEnemyFSM::CAHSEState()
{
	//플레이어를 탐지해서 플레이어를 쫓아가는 상태

	//라인 트레이스 회피를 위해 추가한 코드

	if(!GetWorld()->GetTimerManager().IsTimerActive(LineTraceTimerHandle) )
	{ 
		GetWorld()->GetTimerManager().SetTimer(LineTraceTimerHandle, this, &UDungeonEnemyFSM::PerformLineTrace, LineTraceInterval, true);
	}

	//라인 트레이스 회피를 위해 추가한 코드

	if ( !bIsMovingSideway )
	{
		//플레이어를 목표 지점으로 잡음
		FVector playerLocation = Target->GetActorLocation();
		//플레이어를 향한 벡터를 얻음
		FVector Dir = ( playerLocation - me->GetActorLocation() ).GetSafeNormal();

		FVector MoveDir = Dir;
	
		//플레이어를 향해 움직임
		me->SetActorLocation(me->GetActorLocation() + MoveDir * Speed * GetWorld()->DeltaTimeSeconds , true);

		//플레이어를 향해 바라보는 방향을 얻음
		FRotator NewRot = Dir.Rotation();
		//위아래 회전은 안되게 막음
		NewRot.Pitch = 0;
		//해당 방향을 향해 회전
		me->SetActorRotation(NewRot);

		//만약 플레이어가 공격 범위 안에 있을 경우
		if ( CheckDist(Target->GetActorLocation() , me->GetActorLocation()) < AttackRange )
		{
			//플레이어 공격 상태로 변환
			mState = EDungeonEnemyState::ATTACK;
			Anim->AnimState=mState;
		}
	}
	else 
	{
		CurSideTime += GetWorld()->GetDeltaSeconds();

		if ( !bSideRotation )
		{
			SideDirection = bMoveRight ? me->GetActorRightVector() : -me->GetActorRightVector();
			bSideRotation = true;
			FRotator NewRot = SideDirection.Rotation();
			NewRot.Pitch = 0;
			me->SetActorRotation(NewRot);
		}

		FVector NewLocation = me->GetActorLocation() + SideDirection * Speed * GetWorld()->GetDeltaSeconds();
		me->SetActorLocation(NewLocation , true);

		

		if ( CurSideTime >= SideTime )
		{
			bIsMovingSideway = false;
			bSideRotation = false;
			CurSideTime = 0.0f;
		}
	}

}

void UDungeonEnemyFSM::ATTACKState()
{
	//공격 상태가 되면 공격 애니메이션이 재생
	//공격 애니메이션이 재생되면 공격함수를 불러 플레이어를 공격
	Anim->AnimState = mState;
}

void UDungeonEnemyFSM::DAMAGEState()
{
	//피격 애니메이션을 재생

	//그리고 추격 상태로 전환
}

void UDungeonEnemyFSM::BREAKState()
{
	//넘어지는 애니메이션 재생

	//일어서는 동작이 나올 경우 다시 CHASE상태로 전환
}

void UDungeonEnemyFSM::DIEState()
{
	//사망 애니메이션 재생
	Anim->AnimState = mState;
}

void UDungeonEnemyFSM::PerformLineTrace()
{
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(me);

	for ( float Angle : Angles )
	{
		FRotator rot(0.0f,Angle,0.0f);
		FVector Direction = rot.RotateVector(me->GetActorForwardVector());
		FVector Start = me->GetActorLocation();
		FVector End = Start + me->GetActorForwardVector() * LineTraceDistance;

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult , Start , End , ECC_Pawn , params);

		//DrawDebugLine(GetWorld() , Start , End , FColor::Red , false , 2.0f);

		if ( bHit && hitResult.GetActor()->IsA(ATestEnemy::StaticClass()) )
		{
			bIsMovingSideway = true;
			bMoveRight = FMath::RandBool();
			break;
		}
	}
}
