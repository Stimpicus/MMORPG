// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryDisplayWidget.h"

void UInventoryDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize with placeholder inventory
	InitializePlaceholderInventory();
}

void UInventoryDisplayWidget::InitializePlaceholderInventory()
{
	// Clear existing items
	InventoryItems.Empty();
	
	// Add some placeholder items
	FInventoryItem HealthPotion;
	HealthPotion.ItemName = TEXT("Health Potion");
	HealthPotion.Quantity = 5;
	HealthPotion.ItemType = TEXT("Consumable");
	HealthPotion.Description = TEXT("Restores 50 HP");
	HealthPotion.ItemValue = 25;
	InventoryItems.Add(HealthPotion);

	FInventoryItem ManaPotion;
	ManaPotion.ItemName = TEXT("Mana Potion");
	ManaPotion.Quantity = 3;
	ManaPotion.ItemType = TEXT("Consumable");
	ManaPotion.Description = TEXT("Restores 30 MP");
	ManaPotion.ItemValue = 20;
	InventoryItems.Add(ManaPotion);

	FInventoryItem IronOre;
	IronOre.ItemName = TEXT("Iron Ore");
	IronOre.Quantity = 10;
	IronOre.ItemType = TEXT("Material");
	IronOre.Description = TEXT("Raw iron ore for crafting");
	IronOre.ItemValue = 5;
	InventoryItems.Add(IronOre);

	// Fill remaining slots with empty items
	while (InventoryItems.Num() < MaxInventorySlots)
	{
		FInventoryItem EmptySlot;
		InventoryItems.Add(EmptySlot);
	}
}

void UInventoryDisplayWidget::RefreshInventory()
{
	// Placeholder: In the future, this will fetch data from inventory manager
	// For now, just ensure we have the right number of slots
	while (InventoryItems.Num() < MaxInventorySlots)
	{
		FInventoryItem EmptySlot;
		InventoryItems.Add(EmptySlot);
	}
}

bool UInventoryDisplayWidget::AddItem(const FInventoryItem& Item)
{
	// Find first empty slot
	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		if (InventoryItems[i].Quantity == 0)
		{
			InventoryItems[i] = Item;
			return true;
		}
	}
	return false; // Inventory full
}

bool UInventoryDisplayWidget::RemoveItem(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < InventoryItems.Num())
	{
		FInventoryItem EmptySlot;
		InventoryItems[SlotIndex] = EmptySlot;
		return true;
	}
	return false;
}

FInventoryItem UInventoryDisplayWidget::GetItemAtSlot(int32 SlotIndex) const
{
	if (SlotIndex >= 0 && SlotIndex < InventoryItems.Num())
	{
		return InventoryItems[SlotIndex];
	}
	return FInventoryItem();
}

int32 UInventoryDisplayWidget::GetEmptySlotCount() const
{
	int32 EmptyCount = 0;
	for (const FInventoryItem& Item : InventoryItems)
	{
		if (Item.Quantity == 0)
		{
			++EmptyCount;
		}
	}
	return EmptyCount;
}

bool UInventoryDisplayWidget::IsInventoryFull() const
{
	return GetEmptySlotCount() == 0;
}

void UInventoryDisplayWidget::SaveInventoryData()
{
	// Stub for future persistent storage implementation
	// Will save to game save file or server
}

void UInventoryDisplayWidget::LoadInventoryData()
{
	// Stub for future persistent storage implementation
	// Will load from game save file or server
}
