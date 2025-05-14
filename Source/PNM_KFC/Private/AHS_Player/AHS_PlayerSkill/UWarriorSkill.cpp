

#include "AHS_Player/AHS_PlayerSkill/UWarriorSkill.h"
#include "AHS_Player/PlayerCharacter.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerSkillData.h"
#include "ODH_Enemy/TestEnemy.h"
#include "ODH_Boss/BossEnemy.h"
#include "AHS_Player/PlayerAnimInstance.h"
#include "AHS_PlayerComponent/AC_PlayerDamageManagerComponent.h"


UWarriorSkill::UWarriorSkill()
{

}


UWarriorSkill::~UWarriorSkill()
{

}

// 일반 공격 로직
void UWarriorSkill::UseAttack(APlayerCharacter* Character)
{

	//UE_LOG(LogTemp , Warning , TEXT("전사 일반 공격 실행"));


	APlayerCharacter* OwnerCharacter = Character;
	if ( !OwnerCharacter ) return;

	//--------------------------------------------------------------------
	// [ Sphere trace 구현]

	UWorld* World = OwnerCharacter->GetWorld();
	if ( !World ) return;

	// === Trace 시작 위치 (일단은 플레이어 중심으로) ===
	FVector TraceStart = OwnerCharacter->GetActorLocation();
	float TraceRadius = 200.f;

	// === Sphere Trace 설정 ===
	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

	bool bHit = World->SweepMultiByChannel(
		HitResults ,
		TraceStart ,
		TraceStart ,
		FQuat::Identity ,
		ECC_Pawn , // 또는 ECC_GameTraceChannel1 같은 커스텀 채널
		Sphere
	);

	// === 시각화 ===
	//DrawDebugSphere(World , TraceStart , TraceRadius , 16 , FColor::Red , false , 1.0f);

	bool bEnemyHit = false;

	// === 결과 처리 ===
	if ( bHit )
	{
		for ( const FHitResult& Hit : HitResults )
		{
			AActor* HitActor = Hit.GetActor();
			if ( HitActor && HitActor != OwnerCharacter )
			{
				// === 방식 1: 클래스 체크 ===
				if ( HitActor->IsA(ATestEnemy::StaticClass()))
				{
					UE_LOG(LogTemp , Warning , TEXT("적이 맞습니다: %s") , *HitActor->GetName());

					bEnemyHit = true;


					// 임시
					// 일반 적에게 데미지 로직 적용					
					OwnerCharacter->PlayerDamageManageComp->ApplyDamageToEnemy(20);
				}
				else if ( HitActor->IsA(ABossEnemy::StaticClass()) ) {
					// 보스 적에게 데미지 로직 적용					
					OwnerCharacter->PlayerDamageManageComp->ApplyDamageToEnemy(30);
				}
			}
		}
	}

	// === 시각화: 초록색(적 히트), 빨간색(아무것도 없음) ===
	//FColor DebugColor = bEnemyHit ? FColor::Green : FColor::Red;
	//DrawDebugSphere(World , TraceStart , TraceRadius , 16 , DebugColor , false , 1.0f);
}


//-----------------------------------------------------------------------------------
void UWarriorSkill::SetSkillDataList(const TArray<FPlayerSkillData>& Skills)
{
	WarriorSkills = Skills;

	// 로그로 잘 받아와졌는지 확인
	/*
	UE_LOG(LogTemp , Warning , TEXT("전사 스킬 데이터 리스트 세팅 완료"));

	for ( int32 i = 0; i < WarriorSkills.Num(); ++i )
	{
		const FPlayerSkillData& Skill = WarriorSkills[i];
		UE_LOG(LogTemp , Warning , TEXT("Skill [%d]: ID = %s, Name = %s, Description = %s") ,
			i ,
			*Skill.SkillID.ToString() ,
			*Skill.SkillName.ToString() ,
			*Skill.SkillDescription.ToString() // 만약 Description이 있다면
		);
	}
	*/
}


void UWarriorSkill::UseSkill(APlayerCharacter* Character , int32 SkillIndex)
{
	if ( WarriorSkills.IsValidIndex(SkillIndex) )
	{
		const FPlayerSkillData& Skill = WarriorSkills[SkillIndex];

		//UE_LOG(LogTemp , Warning , TEXT("Use Skill: %s") , *Skill.SkillName.ToString());

		// 스킬 애니메이션 재생
		UPlayerAnimInstance* anim = Cast<UPlayerAnimInstance>(Character->GetMesh()->GetAnimInstance());


		if ( Skill.SkillID == TEXT("WARRIOR_SLASH") )
		{
			// 연속 베기
			anim->PlayWarriorSkillAttack(0);

			// Slash 스킬 실행
			UE_LOG(LogTemp , Warning , TEXT("Use Skill: %s") , *Skill.SkillID.ToString());
		}
		else if ( Skill.SkillID == TEXT("WARRIOR_SMASH") )
		{
			// 블레이드 스메시
			anim->PlayWarriorSkillAttack(1);

			// ShieldBash 스킬 실행
			UE_LOG(LogTemp , Warning , TEXT("Use Skill: %s") , *Skill.SkillID.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Invalid Skill Index: %d") , SkillIndex);
	}
}

