#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FWeaponData.h"
#include "AHS_PlayerComponent/AC_PlayerClassComponent.h"
#include "WeaponInstance.generated.h"

UCLASS(Blueprintable)
class PNM_KFC_API UWeaponInstance : public UObject
{
	GENERATED_BODY()


//-------------------------------------------------------------------------
public:
	UPROPERTY()
	FWeaponData CachedWeaponData; // 저장된 데이터


//-------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	FText GetWeaponName() const { return CachedWeaponData.WeaponName; }

	UFUNCTION(BlueprintCallable)
	int32 GetBaseATKPower() const { return CachedWeaponData.BaseATKPower; }
	
	UFUNCTION(BlueprintCallable)
	EWeaponRarity GetWeaponRarity() const { return CachedWeaponData.WeaponRarity; }

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetWeaponImage() const { return CachedWeaponData.WeaponImage; }

	UFUNCTION(BlueprintCallable)
	FText GetWeaponDescription() const {return CachedWeaponData.WeaponDescription; }

	UFUNCTION(BlueprintCallable)
	EWeaponClassName GetWeaponClassName() const {return CachedWeaponData.ClassName; }


//-------------------------------------------------------------------------
// 무기 장착 및 해제
public:
	UFUNCTION()
	virtual void OnEquip();

	UFUNCTION()
	virtual void OnUnequip();


	static EPlayerClassType ConvertWeaponClassToPlayerClass(EWeaponClassName WeaponClass);


//-------------------------------------------------------------------------
	
public:
	// 무기 고유 ID (플레이어 인벤토리 내에서 유일한 키)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Instance")
	FGuid InstanceID;

	// 참조하는 무기 데이터 테이블 RowName
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Instance")
	FName WeaponRowID;

	// 현재 durability (변화 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Instance")
	int32 CurrentDurability;

	// 강화 수치 (예: +3, +10)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Instance")
	int32 UpgradeLevel;

	// 사용자 지정 외형 (없으면 기본 무기 메쉬 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Instance")
	TSoftObjectPtr<UStaticMesh> OverrideMesh;

	// 생성자
	void InitializeInstance(const FName& InRowID, const FWeaponData& WeaponData);

	// 실제 사용할 메쉬 반환
	UStaticMesh* GetFinalMesh(const FWeaponData& WeaponData) const;
};
