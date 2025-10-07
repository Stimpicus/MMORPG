// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquipmentManagerComponent.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

UEquipmentItem* UEquipmentManagerComponent::EquipItem(UEquipmentItem* Item)
{
	if (!Item || Item->EquipmentSlot == EEquipmentSlot::None)
	{
		return nullptr;
	}

	if (!CanEquipItem(Item))
	{
		return nullptr;
	}

	EEquipmentSlot Slot = Item->EquipmentSlot;
	UEquipmentItem* PreviousItem = nullptr;

	// Check if there's already an item in this slot
	if (EquippedItems.Contains(Slot))
	{
		PreviousItem = EquippedItems[Slot];
		if (PreviousItem)
		{
			PreviousItem->RemoveModifiers();
		}
	}

	// Equip the new item
	EquippedItems.Add(Slot, Item);
	Item->ApplyModifiers();

	// Broadcast equipment change event
	OnEquipmentChanged.Broadcast(Slot, Item);

	return PreviousItem;
}

UEquipmentItem* UEquipmentManagerComponent::UnequipItem(EEquipmentSlot Slot)
{
	if (!EquippedItems.Contains(Slot))
	{
		return nullptr;
	}

	UEquipmentItem* Item = EquippedItems[Slot];
	if (Item)
	{
		Item->RemoveModifiers();
		EquippedItems.Remove(Slot);

		// Broadcast equipment change event
		OnEquipmentChanged.Broadcast(Slot, nullptr);
	}

	return Item;
}

UEquipmentItem* UEquipmentManagerComponent::GetEquippedItem(EEquipmentSlot Slot) const
{
	if (EquippedItems.Contains(Slot))
	{
		return EquippedItems[Slot];
	}
	return nullptr;
}

bool UEquipmentManagerComponent::IsSlotOccupied(EEquipmentSlot Slot) const
{
	return EquippedItems.Contains(Slot) && EquippedItems[Slot] != nullptr;
}

TMap<EEquipmentSlot, UEquipmentItem*> UEquipmentManagerComponent::GetAllEquippedItems() const
{
	return EquippedItems;
}

TArray<UEquipmentItem*> UEquipmentManagerComponent::UnequipAll()
{
	TArray<UEquipmentItem*> UnequippedItems;

	for (auto& Pair : EquippedItems)
	{
		if (Pair.Value)
		{
			Pair.Value->RemoveModifiers();
			UnequippedItems.Add(Pair.Value);

			// Broadcast equipment change event
			OnEquipmentChanged.Broadcast(Pair.Key, nullptr);
		}
	}

	EquippedItems.Empty();
	return UnequippedItems;
}

bool UEquipmentManagerComponent::CanEquipItem(UEquipmentItem* Item) const
{
	if (!Item)
	{
		return false;
	}

	// Additional checks can be added here (level requirements, class restrictions, etc.)
	// For now, just check if it's a valid equipment slot
	return Item->EquipmentSlot != EEquipmentSlot::None;
}
