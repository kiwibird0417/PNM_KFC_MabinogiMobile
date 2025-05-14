#include "ODH_Enemy/EnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "AHS_Player/PlayerCharacter.h"
#include "ODH_EnemySpawner/DungeonEnemySpawner.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 추적을 위한 임시 플레이어 가져오는 코드
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	if ( actor )
	{
		Target = Cast<APlayerCharacter>(actor);
	}

	me = Cast<AActor>(GetOwner());

	CurHP = MaxHP;
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0,1.0f,FColor::Red, logMsg);

	if(bIsPatrol )
	GEngine->AddOnScreenDebugMessage(0 , 1.0f , FColor::Blue , L"true");

	//체력이 끝난 경우
	if ( CurHP <= 0 )
	{
		//사망 상태로 변환
		mState = EEnemyState::DIE;
	}

	switch (mState)
	{
	case EEnemyState::IDLE: {IDLEState();} break;
	case EEnemyState::PATROL: { PATROLState(); } break;
	case EEnemyState::CHASE: { CAHSEState(); } break;
	case EEnemyState::ATTACK: { ATTACKState(); } break;
	case EEnemyState::DAMAGE: { DAMAGEState(); } break;
	case EEnemyState::DIE: { DIEState(); } break;
	}
}

float UEnemyFSM::RandomX(float locationX)
{
	return FMath::RandRange(locationX - SpawnerRadius , locationX + SpawnerRadius);
}

float UEnemyFSM::RandomY(float locationY)
{
	return FMath::RandRange(locationY - SpawnerRadius , locationY + SpawnerRadius);
}

void UEnemyFSM::IDLEState()
{
	//시간이 흐르다가
	CurTime +=GetWorld()->DeltaTimeSeconds;

	//만약 경과 시간이 대기시간을 초과했다면
	if ( CurTime >= IDLEDelayTime )
	{
		int32 ranNum = FMath::RandRange(1,10);

		//만약 랜덤 숫자가 정해진 값보다 크다면
		if ( ranNum > RandomRate )
		{
			//배회 상태로 전환
			mState = EEnemyState::PATROL;
			//경과시간은 초기화
			CurTime = 0.0f;
		}
		//랜덤 숫자가 정해진 값보다 작을 경우
		else
		{	
			//대기 상태로 전환
			mState = EEnemyState::IDLE;
			//경과시간은 초기화
			CurTime = 0.0f;
		}
	}

}

void UEnemyFSM::PATROLState()
{
	//현재 이동 목표가 정해진 배회상태이면
	if ( bIsPatrol )
	{
		FVector CurLocation = me->GetActorLocation();
		FVector Dir = (TargetLocation - CurLocation).GetSafeNormal();

		FVector NewLocation = CurLocation + Dir * Speed * GetWorld()->DeltaTimeSeconds;

		//플레이어를 향해 바라보는 방향을 얻음
		FRotator NewRot = Dir.Rotation();
		//위아래 회전은 안되게 막음
		NewRot.Pitch = 0;
		//해당 방향을 향해 회전
		me->SetActorRotation(NewRot);

		//해당 위치로 이동시킴
		me->SetActorLocation( NewLocation, true);

		//해당 위치와 자신의 위치를 계산함
		float Distance = FVector::Distance(TargetLocation ,me->GetActorLocation());

		//어느 정도 가까워질 경우
		if ( Distance <= 420.0f )
		{
			//현재 불을 초기화
			bIsPatrol = false;

			//대기 상태로 전환
			mState = EEnemyState::IDLE;
		}
	}

	else
	{
		//현재 주인의 위치를 얻어옴
		FVector CurLocation = me->GetActorLocation();

		TargetLocation = FVector(RandomX(Spawner->GetActorLocation().X), RandomX(Spawner->GetActorLocation().Y), Spawner->GetActorLocation().Z);

		bIsPatrol = true;
	}
}

void UEnemyFSM::CAHSEState()
{
	//플레이어를 목표 지점으로 잡음
	FVector playerLocation = Target->GetActorLocation();
	//플레이어를 향한 벡터를 얻음
	FVector Dir = (playerLocation - me->GetActorLocation()).GetSafeNormal();
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
		mState = EEnemyState::ATTACK;
	}
}

void UEnemyFSM::ATTACKState()
{
	//공격 상태가 되면 공격 애니메이션이 재생

	//공격 애니메이션이 재생되면 플레이어에게 공격이 들어감>>변경할지 말지 생각중
	
	//거리가 아직 공격범위 안이면 공격 상태를 이어감
	if ( FVector::Distance(me->GetActorLocation() , Target->GetActorLocation()) < AttackRange )
	{
		return;
	}
	//공격이 끝난 후 플레이어와 거리가 멀면 다시 추격 상태로 전환
	mState = EEnemyState::CHASE;
}

void UEnemyFSM::DAMAGEState()
{
	//자신이 속한 에너미 리스트들에게 공격당했음을 알려줌
	//Spawner->EnemyOnDamage(ListNum);
	
	//해당 상태가 됐을 경우 피격 애니메이션을 재생

	//그리고 추격상태로 전환해줌
	mState = EEnemyState::CHASE;
}

void UEnemyFSM::BREAKState()
{
	//넘어지는 애니메이션 재생

	//일어서는 동작이 나올 경우 다시 CHASE상태로 전환
}

void UEnemyFSM::DIEState()
{
	//사망 애니메이션 재생

	//그리고 스포너에게 자신이 죽었다는 정보를 알려줌
	if ( Spawner )
	{
		
	}
}

