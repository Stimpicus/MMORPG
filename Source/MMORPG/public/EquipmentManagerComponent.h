// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentModifier.h"
#include "EquipmentManagerComponent.generated.h"

// Forward declarations
class UPlayerAttributesComponent;
class UPlayerSkillsComponent;

/**
 * Component that manages equipment and applies modifiers to attributes and skills.
 * This component coordinates between equipment items and the player's attributes/skills.
#include "EquipmentItem.h"
#include "EquipmentManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, Slot, UEquipmentItem*, Item);

/**
 * Component that manages equipped items for a character
 * Handles equip/unequip logic and tracks all equipment slots
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent();

	virtual void BeginPlay() override;

	// Apply an equipment modifier
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ApplyEquipmentModifier(const FEquipmentModifier& Modifier);

	// Remove an equipment modifier by ID
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void RemoveEquipmentModifier(FName ModifierID);

	// Clear all equipment modifiers
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ClearAllModifiers();

	// Get all active modifiers
	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<FEquipmentModifier> GetActiveModifiers() const { return ActiveModifiers; }

protected:
	// Currently active equipment modifiers
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	TArray<FEquipmentModifier> ActiveModifiers;

	// Cached component references
	UPROPERTY()
	UPlayerAttributesComponent* AttributesComponent;

	UPROPERTY()
	UPlayerSkillsComponent* SkillsComponent;

	// Recalculate and apply all modifiers
	void RecalculateModifiers();

	// Cache component references
	void CacheComponentReferences();
protected:
	virtual void BeginPlay() override;

	// Map of equipment slots to equipped items
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TMap<EEquipmentSlot, UEquipmentItem*> EquippedItems;

public:
	// Event fired when equipment changes
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

	/**
	 * Equip an item to the appropriate slot
	 * @param Item The equipment item to equip
	 * @return The previously equipped item (if any)
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquipmentItem* EquipItem(UEquipmentItem* Item);

	/**
	 * Unequip an item from a specific slot
	 * @param Slot The equipment slot to unequip
	 * @return The unequipped item
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquipmentItem* UnequipItem(EEquipmentSlot Slot);

	/**
	 * Get the item equipped in a specific slot
	 * @param Slot The equipment slot to query
	 * @return The equipped item, or nullptr if slot is empty
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	UEquipmentItem* GetEquippedItem(EEquipmentSlot Slot) const;

	/**
	 * Check if a slot has an item equipped
	 * @param Slot The equipment slot to check
	 * @return True if an item is equipped in the slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool IsSlotOccupied(EEquipmentSlot Slot) const;

	/**
	 * Get all equipped items
	 * @return Map of all equipped items by slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	TMap<EEquipmentSlot, UEquipmentItem*> GetAllEquippedItems() const;

	/**
	 * Unequip all items
	 * @return Array of all unequipped items
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	TArray<UEquipmentItem*> UnequipAll();

	/**
	 * Check if an item can be equipped
	 * @param Item The item to check
	 * @return True if the item can be equipped
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool CanEquipItem(UEquipmentItem* Item) const;
};
