// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, UInventoryComponent*, Inventory);

/**
 * Structure representing an inventory slot
 */
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	FInventorySlot()
		: Item(nullptr)
		, Quantity(0)
	{
	}

	FInventorySlot(UItem* InItem, int32 InQuantity)
		: Item(InItem)
		, Quantity(InQuantity)
	{
	}
};

/**
 * Component that manages an inventory of items
 * Handles adding, removing, and sorting items
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	float MaxWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	float CurrentWeight;

public:
	// Event fired when inventory changes
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	/**
	 * Add an item to the inventory
	 * @param Item The item to add
	 * @param Quantity The quantity to add
	 * @return True if the item was successfully added
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItem* Item, int32 Quantity = 1);

	/**
	 * Remove an item from the inventory
	 * @param Item The item to remove
	 * @param Quantity The quantity to remove
	 * @return True if the item was successfully removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UItem* Item, int32 Quantity = 1);

	/**
	 * Remove an item from a specific slot
	 * @param SlotIndex The index of the slot
	 * @param Quantity The quantity to remove
	 * @return True if the item was successfully removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromSlot(int32 SlotIndex, int32 Quantity = 1);

	/**
	 * Get an item from a specific slot
	 * @param SlotIndex The index of the slot
	 * @return The inventory slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventorySlot GetItemAtSlot(int32 SlotIndex) const;

	/**
	 * Find an item in the inventory
	 * @param Item The item to find
	 * @return The index of the first slot containing the item, or -1 if not found
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindItem(UItem* Item) const;

	/**
	 * Check if the inventory has enough of an item
	 * @param Item The item to check
	 * @param Quantity The required quantity
	 * @return True if the inventory has enough of the item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(UItem* Item, int32 Quantity = 1) const;

	/**
	 * Get the total quantity of an item in the inventory
	 * @param Item The item to count
	 * @return The total quantity
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemQuantity(UItem* Item) const;

	/**
	 * Sort the inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortInventory();

	/**
	 * Get the number of empty slots
	 * @return The number of empty slots
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetEmptySlotCount() const;

	/**
	 * Check if the inventory is full
	 * @return True if the inventory is full
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsFull() const;

	/**
	 * Get the current weight of all items in the inventory
	 * @return The current weight
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetCurrentWeight() const;

	/**
	 * Clear the entire inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();

	// Persistent storage stubs for future SpacetimeDB integration
	
	/**
	 * Save inventory state to persistent storage
	 * Stub for future SpacetimeDB integration
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Persistence")
	void SaveInventoryState();

	/**
	 * Load inventory state from persistent storage
	 * Stub for future SpacetimeDB integration
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Persistence")
	void LoadInventoryState();

	/**
	 * Serialize inventory to a JSON string
	 * Helper for persistence
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Persistence")
	FString SerializeInventory() const;

	/**
	 * Deserialize inventory from a JSON string
	 * Helper for persistence
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Persistence")
	bool DeserializeInventory(const FString& JsonString);

private:
	void UpdateWeight();
	int32 FindEmptySlot() const;
	int32 FindStackableSlot(UItem* Item) const;
};
