

#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "AHS_Player/AHS_PlayerSkill/PlayerSkillData.h"
#include "AHS_Player/PlayerCharacter.h"
#include "AHS_UI/PlayerSkillUI.h"
#include "AHS_PlayerComponent/AC_PlayerSkillComponent.h"
#include "AHS_Player/PlayerAnimInstance.h"



// Sets default values for this component's properties
UAC_PlayerClassComponent::UAC_PlayerClassComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); // 이거 꼭 필요!
}



void UAC_PlayerClassComponent::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 컨트롤러 불러오기
	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());

	
}


// Called every frame
void UAC_PlayerClassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



}

//void UAC_PlayerClassComponent::SetupClassDefaults()
//{
//	OnRep_PlayerClass();
//
//	if ( OwnerCharacter )
//	{
//		OwnerCharacter->OnRep_WeaponMesh();
//	}
//	else
//	{
//		UE_LOG(LogTemp , Warning , TEXT("OwnerCharacter is null!"));
//	}
//}

void UAC_PlayerClassComponent::SetupClassDefaults()
{
	OnRep_PlayerClass(); // 여전히 로컬에선 클래스 적용됨

	if ( OwnerCharacter && OwnerCharacter->HasAuthority() )
	{
		//OwnerCharacter->Server_ApplyWeaponMesh(); // 서버에서 Multicast를 트리거
		OwnerCharacter->ServerRPC_SetWeaponMesh();
	}
}



void UAC_PlayerClassComponent::OnRep_PlayerClass()
{
	SetupClassDefaults_Logic(); // 클라이언트에서도 실행됨
	//OwnerCharacter->OnRep_WeaponMesh();
}


void UAC_PlayerClassComponent::SetupClassDefaults_Logic()
{
	static const FString DataTablePath = TEXT("/Script/Engine.DataTable'/Game/AHS/Data/ClassDataTables/DT_AllPlayerSkillData.DT_AllPlayerSkillData'");
	UDataTable* SkillDataTable = LoadObject<UDataTable>(nullptr , *DataTablePath);

	if ( !SkillDataTable )
	{
		UE_LOG(LogTemp , Error , TEXT("Failed to load unified skill DataTable"));
		return;
	}


	TArray<FPlayerSkillData*> AllSkills;
	SkillDataTable->GetAllRows<FPlayerSkillData>(TEXT("SkillContext") , AllSkills);


	FString ClassNameString = UEnum::GetValueAsString(PlayerClass);
	ClassNameString = ClassNameString.RightChop(ClassNameString.Find(TEXT("::")) + 2);

	int32 DisplayIndex = 0;

	//---------------------------------------------------------------------------------------------------------
	// 스킬 UI 초기화

	if ( OwnerCharacter->IsLocallyControlled() )
	{
		if ( OwnerCharacter->skillUI )
		{
			for ( int32 i = 0; i < 6; ++i )
			{
				OwnerCharacter->skillUI->SetSkillNameText(i , FText::GetEmpty());
			}
		}
	}




	// 기존 스킬 초기화
	ClassSkills.Empty();

	//----------------------------------------------------------------------------------------------------------
	for ( const FPlayerSkillData* Skill : AllSkills )
	{
		if ( Skill && Skill->ClassName.ToString() == ClassNameString )
		{
			// 요구 레벨보다 플레이어 레벨이 낮으면 등록 안 함
			if ( OwnerCharacter->playerLevel < Skill->RequiredPlayerLevel )
			{
				continue;
			}


			// 저장
			ClassSkills.Add(*Skill);

			// UI 적용
			if ( OwnerCharacter->skillUI && DisplayIndex < 6 )
			{

				// 플레이어 캐릭터 레벨보다 같거나 작은 스킬만 등록

				// 스킬 텍스트 설정
				OwnerCharacter->skillUI->SetSkillNameText(DisplayIndex , Skill->SkillName);
				//UE_LOG(LogTemp , Warning , TEXT("Index i : %d, Skill Name : %s") , DisplayIndex , *Skill->SkillName.ToString());

				// 아이콘 설정
				if ( Skill->SkillIcon )
				{
					OwnerCharacter->skillUI->SetSkillIcon(DisplayIndex , Skill->SkillIcon , true);
				}
				else
				{
					// 아이콘이 없으면 기본 빈 아이콘 사용
					OwnerCharacter->skillUI->SetSkillIcon(DisplayIndex , nullptr , false);
				}
			}

			++DisplayIndex;

			if ( DisplayIndex >= 6 )
				break;
		}
	}

	if ( OwnerCharacter && OwnerCharacter->PlayerSkillComp )
	{
		OwnerCharacter->PlayerSkillComp->SetSkillDataList(ClassSkills);
		OwnerCharacter->PlayerSkillComp->SetSkillStrategy(PlayerClass);
	}

	// 공격 범위 바꿔주기
	switch ( PlayerClass )
	{
	case EPlayerClassType::Warrior: { OwnerCharacter->SetAttackRange(200.0f); }	break;
	case EPlayerClassType::Mage: { OwnerCharacter->SetAttackRange(600.0f); }	break;
	}

	//----------------------------------------------------------------------------------------------------------
	// Anim Instance(Animation Blueprint)에도, 플레이어 클래스 갱신
	auto anim = Cast<UPlayerAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	anim->PlayerClass = PlayerClass;



	//----------------------------------------------------------------------------------------------------------
}
