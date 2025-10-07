// Copyright Epic Games, Inc. All Rights Reserved.

// This is an EXAMPLE file showing how to use the Inventory and Equipment systems
// You can use this as a reference for implementing these systems in your character class

/*
Example Character Header (MyPlayerCharacter.h):

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "EquipmentManagerComponent.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class MMORPG_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	// Inventory and Equipment Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UEquipmentManagerComponent* EquipmentManagerComponent;

public:
	// Example interaction methods
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PickupItem(UItem* Item, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItemFromInventory(UEquipmentItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipItemToInventory(EEquipmentSlot Slot);

	// Event handlers
	UFUNCTION()
	void OnEquipmentChangedHandler(EEquipmentSlot Slot, UEquipmentItem* Item);

	UFUNCTION()
	void OnInventoryChangedHandler(UInventoryComponent* Inventory);
};
*/

/*
Example Character Implementation (MyPlayerCharacter.cpp):

#include "MyPlayerCharacter.h"
#include "EquipmentItem.h"

AMyPlayerCharacter::AMyPlayerCharacter()
{
	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InventoryComponent->MaxSlots = 30;
	InventoryComponent->MaxWeight = 100.0f;

	// Create equipment manager component
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManager"));
}

void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind to component events
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.AddDynamic(this, &AMyPlayerCharacter::OnInventoryChangedHandler);
	}

	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->OnEquipmentChanged.AddDynamic(this, &AMyPlayerCharacter::OnEquipmentChangedHandler);
	}
}

void AMyPlayerCharacter::PickupItem(UItem* Item, int32 Quantity)
{
	if (!Item || !InventoryComponent)
	{
		return;
	}

	bool bSuccess = InventoryComponent->AddItem(Item, Quantity);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Picked up %d x %s"), Quantity, *Item->ItemName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to pick up %s - inventory full or too heavy"), *Item->ItemName);
	}
}

void AMyPlayerCharacter::EquipItemFromInventory(UEquipmentItem* Item)
{
	if (!Item || !InventoryComponent || !EquipmentManagerComponent)
	{
		return;
	}

	// Check if we have the item in inventory
	if (!InventoryComponent->HasItem(Item, 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item not in inventory"));
		return;
	}

	// Try to equip the item
	UEquipmentItem* PreviousItem = EquipmentManagerComponent->EquipItem(Item);

	// If successful, remove from inventory
	if (EquipmentManagerComponent->GetEquippedItem(Item->EquipmentSlot) == Item)
	{
		InventoryComponent->RemoveItem(Item, 1);
		UE_LOG(LogTemp, Log, TEXT("Equipped %s"), *Item->ItemName);

		// Add previously equipped item back to inventory
		if (PreviousItem)
		{
			InventoryComponent->AddItem(PreviousItem, 1);
			UE_LOG(LogTemp, Log, TEXT("Unequipped %s to inventory"), *PreviousItem->ItemName);
		}
	}
}

void AMyPlayerCharacter::UnequipItemToInventory(EEquipmentSlot Slot)
{
	if (!InventoryComponent || !EquipmentManagerComponent)
	{
		return;
	}

	// Get the item in the slot
	UEquipmentItem* Item = EquipmentManagerComponent->GetEquippedItem(Slot);
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("No item equipped in that slot"));
		return;
	}

	// Check if we have room in inventory
	if (InventoryComponent->IsFull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full - cannot unequip"));
		return;
	}

	// Unequip the item
	UEquipmentItem* UnequippedItem = EquipmentManagerComponent->UnequipItem(Slot);
	if (UnequippedItem)
	{
		// Add to inventory
		InventoryComponent->AddItem(UnequippedItem, 1);
		UE_LOG(LogTemp, Log, TEXT("Unequipped %s to inventory"), *UnequippedItem->ItemName);
	}
}

void AMyPlayerCharacter::OnEquipmentChangedHandler(EEquipmentSlot Slot, UEquipmentItem* Item)
{
	// This is called whenever equipment changes
	// Update UI, visuals, stats, etc.
	
	if (Item)
	{
		UE_LOG(LogTemp, Log, TEXT("Equipment changed: %s equipped to slot"), *Item->ItemName);
		// Update character mesh/visuals based on equipped item
		// Apply stat bonuses
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Equipment removed from slot"));
		// Remove visuals
		// Remove stat bonuses
	}
}

void AMyPlayerCharacter::OnInventoryChangedHandler(UInventoryComponent* Inventory)
{
	// This is called whenever inventory changes
	// Update inventory UI
	
	UE_LOG(LogTemp, Log, TEXT("Inventory changed - Weight: %.2f / %.2f, Empty Slots: %d"),
		Inventory->GetCurrentWeight(),
		Inventory->MaxWeight,
		Inventory->GetEmptySlotCount());
}
*/

/*
Example Usage in Blueprint or C++:

// Create a potion
UItem* HealthPotion = NewObject<UItem>();
HealthPotion->ItemName = TEXT("Health Potion");
HealthPotion->Description = TEXT("Restores 50 HP");
HealthPotion->ItemID = 1001;
HealthPotion->Weight = 0.5f;
HealthPotion->Value = 25;
HealthPotion->bIsStackable = true;
HealthPotion->MaxStackSize = 20;

// Create an iron helmet
UEquipmentItem* IronHelmet = NewObject<UEquipmentItem>();
IronHelmet->ItemName = TEXT("Iron Helmet");
IronHelmet->Description = TEXT("A sturdy iron helmet");
IronHelmet->ItemID = 2001;
IronHelmet->Weight = 3.0f;
IronHelmet->Value = 100;
IronHelmet->EquipmentSlot = EEquipmentSlot::Head;
IronHelmet->RequiredLevel = 5;
IronHelmet->ArmorRating = 10;

// Add attribute modifier
FAttributeModifier DefenseBonus;
DefenseBonus.AttributeName = TEXT("Defense");
DefenseBonus.ModifierValue = 5.0f;
DefenseBonus.bIsPercentage = false;
IronHelmet->AttributeModifiers.Add(DefenseBonus);

// Pickup items
MyCharacter->PickupItem(HealthPotion, 5);
MyCharacter->PickupItem(IronHelmet, 1);

// Equip the helmet
MyCharacter->EquipItemFromInventory(IronHelmet);

// Later, unequip it
MyCharacter->UnequipItemToInventory(EEquipmentSlot::Head);

// Sort inventory
MyCharacter->InventoryComponent->SortInventory();

// Save inventory (stub - implement when SpacetimeDB is integrated)
MyCharacter->InventoryComponent->SaveInventoryState();
*/

// For Blueprint usage:
// 1. Create a Blueprint based on your character class
// 2. The InventoryComponent and EquipmentManagerComponent will be visible in the components list
// 3. Create Blueprint item classes based on UItem or UEquipmentItem
// 4. Use Blueprint nodes to:
//    - Add items: "Add Item" node on Inventory Component
//    - Equip items: "Equip Item" node on Equipment Manager Component
//    - Check inventory: "Has Item", "Get Item Quantity" nodes
//    - Listen to events: Bind to "On Inventory Changed" and "On Equipment Changed"
