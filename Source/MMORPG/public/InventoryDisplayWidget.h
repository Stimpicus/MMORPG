// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDisplayWidget.generated.h"

/**
 * Simple inventory item structure for placeholder data
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FString ItemName = TEXT("Empty Slot");

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FString ItemType = TEXT("Misc");

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FString Description = TEXT("An item");

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 ItemValue = 0;
};

/**
 * Inventory Display Panel Widget
 * Displays inventory items with management and persistent storage stubs
 */
UCLASS()
class MMORPG_API UInventoryDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Inventory capacity
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySlots = 20;

	// Current inventory items (placeholder data)
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> InventoryItems;

	// Currency
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 Gold = 100;

	// Blueprint-callable functions for future data integration
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FInventoryItem& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItem GetItemAtSlot(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetEmptySlotCount() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull() const;

	// Persistent storage stubs (for future implementation)
	UFUNCTION(BlueprintCallable, Category = "Inventory|Persistence")
	void SaveInventoryData();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Persistence")
	void LoadInventoryData();

protected:
	virtual void NativeConstruct() override;

private:
	void InitializePlaceholderInventory();
};
