// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerSkillData.generated.h"



UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Physical    UMETA(DisplayName = "Physical"),
    Magical     UMETA(DisplayName = "Magical"),
    Support     UMETA(DisplayName = "Support")
};

UENUM(BlueprintType)
enum class ESkillElement : uint8
{
    None        UMETA(DisplayName = "None"),
    Fire        UMETA(DisplayName = "Fire"),
    Ice         UMETA(DisplayName = "Ice"),
    Lightning   UMETA(DisplayName = "Lightning"),
	Ground		UMETA(DisplayName = "Ground")
};

USTRUCT(BlueprintType)
struct FPlayerSkillData : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName ClassName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FText SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FText SkillDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillElement SkillElement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 SkillEffectNum;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 TargetEffectNum;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float CoolTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UTexture2D* SkillIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float SkillDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName AnimationName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 RequiredPlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float SkillDamageMultiplier;

};
