#include "ODH_Boss/BossEnemy.h"
#include "Components/SphereComponent.h"
#include "ODH_Boss/BossFSM.h"
#include "Components/CapsuleComponent.h"
#include "AHS_Player/PlayerCharacter.h"
#include "ODH_EnemyAnim/BossAnim.h"
#include "Components/WidgetComponent.h"
#include "ODH_EnemyUI/EnemyUI.h"
#include "Particles/ParticleSystemComponent.h"

ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	FSM = CreateDefaultSubobject<UBossFSM>(L"FSM");

	SphereComp = CreateDefaultSubobject<USphereComponent>(L"SphereComp");
	SphereComp->SetupAttachment(GetMesh());
	SphereComp->SetSphereRadius(750);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::InteractionPlayer);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0,-4,-142), FRotator(0,0,-90));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(L"/Script/Engine.SkeletalMesh'/Game/ODH/Asset/Boss/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'");
	if ( TempMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnDamege);

	ZPosSceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("ZPosSceneComp"));
	ZPosSceneComp->SetupAttachment(RootComponent);
	ZPosSceneComp->SetRelativeLocation(FVector(0,0,-142));

	StompEffecPos = CreateDefaultSubobject<USceneComponent>(TEXT("StompPosComp"));
	StompEffecPos->SetupAttachment(GetMesh());
	StompEffecPos->SetRelativeLocation(FVector(-120, 150, 20));

	UIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPUIComp"));
	UIComp->SetupAttachment(GetMesh());

	//공격 관련 파티클들
	AttackParticle=CreateDefaultSubobject<UParticleSystemComponent>(L"AttackParticle");
	AttackParticle->SetupAttachment(GetCapsuleComponent());
	AttackParticle->SetRelativeLocationAndRotation(FVector(-80, 150.299228f, -70.052762),FRotator(-80, -90 , 0));
	AttackParticle->SetActive(false);

	SwingParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"SwingParticle");
	SwingParticle->SetupAttachment(GetCapsuleComponent());
	SwingParticle->SetRelativeLocationAndRotation(FVector(80, 149.392318f, 173.054071f), FRotator(0,0,-170));
	SwingParticle->SetActive(false);

// 	MovingParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"MovingParticle");
// 	MovingParticle->SetupAttachment(GetCapsuleComponent());
// 
// 	StompParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"StompParticle");
// 	StompParticle->SetupAttachment(GetCapsuleComponent());
	
	bReplicates = true;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	FSM->LoadStat(TEXT("Boss"));

	EnemyHPUI = Cast<UEnemyUI>(UIComp->GetWidget());

	EnemyHPUI->Name = TEXT("사신");

	UIComp->SetVisibility(false);
}

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossEnemy::OnDamege(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	//FSM->mState = EBossState::DAMAGE;
	//FSM->CurHP-=2;
}

void ABossEnemy::InteractionPlayer(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if(player)
	{
		//첫 조회만 상호작용이 이루어지게 만듦
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//상태를 추적으로 전환
		FSM->mState = EBossState::CHASE;
		//FSM->isTargetPlayer = true;
		FSM->Anim->AnimState = FSM->mState;
		//먼저 시야에 들어온 플레이어를 쫓게 만듦
		FSM->SetTarget(player);

		UIComp->SetVisibility(true);
	}
}

float ABossEnemy::GetHP()
{
	return FSM->CurHP;
}

void ABossEnemy::SetHP(float value)
{
	FSM->CurHP = value;

	float percent = FSM->CurHP / FSM->MaxHP;

	EnemyHPUI->hp = percent;
}

void ABossEnemy::DamageProcess(int32 damage)
{
	float newHP = FSM->CurHP -= damage;
	SetHP(newHP);
}

void ABossEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ABossEnemy , FSM);
	DOREPLIFETIME(ABossEnemy , EnemyHPUI);
}

