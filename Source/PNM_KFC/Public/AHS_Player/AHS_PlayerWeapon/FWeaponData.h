// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FWeaponData.generated.h"


UENUM(BlueprintType)
enum class EWeaponClassName : uint8
{
	Warrior UMETA(DisplayName = "Warrior") ,
	Archer UMETA(DisplayName = "Archer") ,
	Mage UMETA(DisplayName = "Mage") ,
	Healer UMETA(DisplayName = "Healer") ,
	Bard UMETA(DisplayName = "Bard") ,
};


UENUM(BlueprintType)
enum class EWeaponRarity : uint8
{
	None = 0,
	Basic = 1 ,
	Advanced = 2 ,
	Rare = 3 ,
	Elite = 4,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0 ,
	SwordAndShield = 1,
	Shortbow = 2,
	Wand = 3,
	HealingWand = 4,
	Lute = 5,
};

UENUM(BlueprintType)
enum class EMainStatType : uint8
{
	None = 0 ,
	Strength = 1,
	Dexterity = 2,
	Intelligence = 3,
	Willpower = 4,
	Luck = 5,
};


//------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()


	//-------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
    int32 ID;

	//------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	EWeaponClassName ClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
    FText WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* WeaponMesh; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
    UTexture2D* WeaponImage;


	//------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	int32 BaseATKPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	float BonusDamage;

	//-------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	EWeaponRarity WeaponRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	int32 PlayerRequiredLevel;

	//-------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	EMainStatType MainStatType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	float MainStatMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	EMainStatType SubStatType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	float SubStatMultiplier;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	int32 WeaponDurability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	FText WeaponDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	float WeaponWeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	int32 WeaponPrice;


	//-------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	FText Traits;	//TArray<ETraits> Traits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	FText Engraving;	//TSoftObjectPtr<UEngravingData> Engraving;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	FText SkillIndexes;	//TArray<int32> SkillIndexes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	FText Gem;	//TSoftObjectPtr<UGemData> Gem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapon")
	FText WeaponObtainData;	//TSoftObjectPtr<UWeaponObtainData> WeaponObtainData;
};
