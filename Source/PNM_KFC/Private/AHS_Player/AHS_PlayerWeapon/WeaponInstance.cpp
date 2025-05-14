// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS_Player/AHS_PlayerWeapon/WeaponInstance.h"
#include "AHS_Player/PlayerCharacter.h"

void UWeaponInstance::OnEquip()
{
	// 1. 플레이어 얻기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if ( !PC ) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
	if ( !Player ) return;

	//====================================================================
	// 플레이어 직업 갱신
	EPlayerClassType NewClass = ConvertWeaponClassToPlayerClass(CachedWeaponData.ClassName);
	
	Player->PlayerClassComp->PlayerClass = NewClass;

	//0418(금), 플레이어 측으로 코드 옮김.
	//Player->PlayerClassComp->SetupClassDefaults(); // <- 이 함수가 필요

	//======================================================
	// 0418(금), 플레이어 측으로 코드 옮김.
	// 2. 무기 메쉬 장착
	//if ( UStaticMesh* MeshToApply = GetFinalMesh(CachedWeaponData) )
	//{
	//	if ( Player->WeaponComp ) // 이건 PlayerCharacter에 정의돼 있어야 해
	//	{
	//		Player->WeaponComp->SetStaticMesh(MeshToApply);
	//	}
	//}
}

//-------------------------------------------------------------------------
EPlayerClassType UWeaponInstance::ConvertWeaponClassToPlayerClass(EWeaponClassName WeaponClass)
{
	switch ( WeaponClass )
	{
	case EWeaponClassName::Warrior:
		return EPlayerClassType::Warrior;
	case EWeaponClassName::Archer:
		return EPlayerClassType::Archer;
	case EWeaponClassName::Mage:
		return EPlayerClassType::Mage;
	case EWeaponClassName::Healer:
		return EPlayerClassType::Healer;
	case EWeaponClassName::Bard:
		return EPlayerClassType::Bard;
	default:
		return EPlayerClassType::Warrior; // 기본값 처리
	}
}



//-------------------------------------------------------------------------
void UWeaponInstance::OnUnequip()
{

}

//------------------------------------------------------------------------
void UWeaponInstance::InitializeInstance(const FName& InRowID , const FWeaponData& WeaponData)
{
	InstanceID = FGuid::NewGuid();
	WeaponRowID = InRowID;
	CachedWeaponData = WeaponData;

	CurrentDurability = WeaponData.WeaponDurability;
	UpgradeLevel = 0;
	OverrideMesh = nullptr;
}


UStaticMesh* UWeaponInstance::GetFinalMesh(const FWeaponData& WeaponData) const
{
	if ( OverrideMesh.IsValid() )
	{
		UObject* ResolvedObject = OverrideMesh.Get();
		if ( ResolvedObject == nullptr )
		{
			UE_LOG(LogTemp , Error , TEXT("OverrideMesh was valid but resolved to nullptr!"));
		}
		return Cast<UStaticMesh>(ResolvedObject); // 방어적 캐스팅
	}

	if ( WeaponData.WeaponMesh )
	{
		return WeaponData.WeaponMesh;
	}

	UE_LOG(LogTemp , Warning , TEXT("GetFinalMesh: No mesh available."));
	return nullptr;
}


