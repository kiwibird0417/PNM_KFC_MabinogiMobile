#pragma once

#include "CoreMinimal.h"
#include "FInventoryItem.generated.h"

UENUM(BlueprintType)
enum class EInventoryCategory : uint8
{
	Equipment     UMETA(DisplayName = "Equipment"),     // 무기, 방어구 등
	Tool          UMETA(DisplayName = "Tool"),
	Consumable    UMETA(DisplayName = "Item"),
	Fashion       UMETA(DisplayName = "Fashion"),
	Pet           UMETA(DisplayName = "Mount / Pet"),
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	FInventoryItem()
		: Category(EInventoryCategory::Equipment), ItemInstance(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInventoryCategory Category;

	// 각종 인스턴스를 UObject 기반으로 저장 (무기, 도구 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UObject> ItemInstance;
};
