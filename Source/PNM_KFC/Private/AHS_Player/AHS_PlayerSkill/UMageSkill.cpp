

#include "AHS_Player/AHS_PlayerSkill/UMageSkill.h"
#include "AHS_Player/PlayerCharacter.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerSkillData.h"
#include "AHS_Player/AHS_PlayerAttack/MageAttack/NormalProjectile.h"
#include "AHS_Player/PlayerAnimInstance.h"


UMageSkill::UMageSkill()
{
	// ConstructorHelpers는 UObject에서는 안 되고, 그래서 대신 LoadObject 사용
	FStringAssetReference AssetRef(TEXT("/Game/AHS/Blueprints/BP_NormalProjectile.BP_NormalProjectile_C"));
	LoadedProjectileClass = LoadObject<UClass>(nullptr , *AssetRef.ToString());

	if ( !LoadedProjectileClass )
	{
		UE_LOG(LogTemp , Error , TEXT("ProjectileClass 로딩 실패: %s") , *AssetRef.ToString());
	}
	else
	{
		UE_LOG(LogTemp , Log , TEXT("ProjectileClass 성공적으로 로드됨"));
	}
	//==========================================================================
	// 번개(일렉트로) 프로젝타일 로드
	FStringAssetReference ElectroRef(TEXT("/Game/AHS/Blueprints/BP_ElectroProjectile.BP_ElectroProjectile_C"));
	//FStringAssetReference ElectroRef(TEXT("/Game/AHS/Blueprints/BP_NormalProjectile.BP_NormalProjectile_C"));
	ElectroProjectileClass = LoadObject<UClass>(nullptr , *ElectroRef.ToString());

	if ( !ElectroProjectileClass )
	{
		UE_LOG(LogTemp , Error , TEXT("ElectroProjectileClass 로딩 실패: %s") , *ElectroRef.ToString());
	}
	else
	{
		UE_LOG(LogTemp , Log , TEXT("ElectroProjectileClass 성공적으로 로드됨"));
	}

	//==========================================================================

}


UMageSkill::~UMageSkill()
{

}

void UMageSkill::UseAttack(APlayerCharacter* Character)
{
	//UE_LOG(LogTemp , Warning , TEXT("마법사 일반 공격"));

	if ( !Character || !Character->CurrentTarget )
	{
		UE_LOG(LogTemp , Warning , TEXT("Character나 Target이 없음"));
		return;
	}

	UWorld* World = Character->GetWorld();
	if ( !World )
	{
		UE_LOG(LogTemp , Error , TEXT("World가 없음"));
		return;
	}

	if ( LoadedProjectileClass )
	{
		FVector SpawnLocation = Character->GetActorLocation();

		
		FRotator SpawnRotation = Character->GetActorRotation();

		AActor* SpawnedActor = World->SpawnActor<AActor>(LoadedProjectileClass , SpawnLocation , SpawnRotation);

	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("ProjectileClass가 비어 있음!"));
	}
}

void UMageSkill::SetSkillDataList(const TArray<FPlayerSkillData>& Skills)
{
	MageSkills = Skills;
}


void UMageSkill::UseSkill(APlayerCharacter* Character , int32 SkillIndex)
{
	if ( MageSkills.IsValidIndex(SkillIndex) )
	{
		const FPlayerSkillData& Skill = MageSkills[SkillIndex];

		UE_LOG(LogTemp , Warning , TEXT("Use Skill: %s") , *Skill.SkillName.ToString());

		// 스킬 애니메이션 재생
		UPlayerAnimInstance* anim = Cast<UPlayerAnimInstance>(Character->GetMesh()->GetAnimInstance());

		// 여기서 Skill.SkillID 등을 기준으로 분기 처리 가능
		if ( Skill.SkillID == TEXT("MAGE_LIGHTNING") )
		{
			// 전기 스킬 애니메이션
			anim->PlayMageSkillAttack(0);

			UE_LOG(LogTemp , Warning , TEXT("Use Skill: %s") , *Skill.SkillID.ToString());
		}
		else if ( Skill.SkillID == TEXT("MAGE_FIREBALL") )
		{
			// 불 스킬 애니메이션
			anim->PlayMageSkillAttack(1);

			UE_LOG(LogTemp , Warning , TEXT("Use Skill: %s") , *Skill.SkillID.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Invalid Skill Index: %d") , SkillIndex);
	}
}

void UMageSkill::ElectroSkill(APlayerCharacter* Character)
{

	if ( !Character->CurrentTarget )
	{
		UE_LOG(LogTemp , Warning , TEXT("ElectroSkill: 타겟이 없습니다."));
		return;
	}

	UWorld* World =Character->GetWorld();
	if ( !World )
	{
		UE_LOG(LogTemp , Error , TEXT("ElectroSkill: World가 없습니다."));
		return;
	}

	if ( ElectroProjectileClass )
	{
		FVector SpawnLocation = Character->GetActorLocation(); //Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
		//FVector TargetLocation = Character->CurrentTarget->GetActorLocation();
		FRotator SpawnRotation = Character->GetActorRotation(); //( TargetLocation - SpawnLocation ).Rotation();

		AActor* SpawnedActor = World->SpawnActor<AActor>(ElectroProjectileClass , SpawnLocation , SpawnRotation);



		if ( SpawnedActor )
		{
			UE_LOG(LogTemp , Log , TEXT(" ElectroProjectile 생성 성공!"));
			UE_LOG(LogTemp , Log , TEXT("ElectroProjectile 생성 위치: %s") , *SpawnedActor->GetActorLocation().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("ElectroSkill: ElectroProjectileClass가 비어있습니다!"));
	}

}



void UMageSkill::FireSkill()
{

}

