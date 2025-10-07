// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"
#include "EquipmentItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxSlots = 30;
	MaxWeight = 100.0f;
	CurrentWeight = 0.0f;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UInventoryComponent::AddResource(EResourceType ResourceType, int32 Quantity)
{
	if (ResourceType == EResourceType::None || Quantity <= 0)
	
	// Initialize inventory slots
	InventorySlots.SetNum(MaxSlots);
}

bool UInventoryComponent::AddItem(UItem* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		return false;
	}

	// Check if we already have this resource
	if (ResourceInventory.Contains(ResourceType))
	{
		int32 CurrentQuantity = ResourceInventory[ResourceType];
		
		// Check max stack size if applicable
		if (MaxStackSize > 0)
		{
			int32 SpaceAvailable = MaxStackSize - CurrentQuantity;
			if (SpaceAvailable <= 0)
			{
				return false;
			}
			
			// Add only what fits
			int32 AmountToAdd = FMath::Min(Quantity, SpaceAvailable);
			ResourceInventory[ResourceType] = CurrentQuantity + AmountToAdd;
		}
		else
		{
			// No stack limit, add full amount
			ResourceInventory[ResourceType] = CurrentQuantity + Quantity;
		}
	}
	else
	{
		// New resource type
		if (MaxStackSize > 0)
		{
			int32 AmountToAdd = FMath::Min(Quantity, MaxStackSize);
			ResourceInventory.Add(ResourceType, AmountToAdd);
		}
		else
		{
			ResourceInventory.Add(ResourceType, Quantity);
		}
	}

	return true;
}

bool UInventoryComponent::AddResourceItem(const FResourceItem& ResourceItem)
{
	return AddResource(ResourceItem.ResourceType, ResourceItem.Quantity);
}

int32 UInventoryComponent::RemoveResource(EResourceType ResourceType, int32 Quantity)
{
	if (!ResourceInventory.Contains(ResourceType) || Quantity <= 0)
	{
		return 0;
	}

	int32 CurrentQuantity = ResourceInventory[ResourceType];
	int32 AmountToRemove = FMath::Min(Quantity, CurrentQuantity);

	ResourceInventory[ResourceType] = CurrentQuantity - AmountToRemove;

	// Remove entry if quantity is now zero
	if (ResourceInventory[ResourceType] <= 0)
	{
		ResourceInventory.Remove(ResourceType);
	}

	return AmountToRemove;
}

int32 UInventoryComponent::GetResourceQuantity(EResourceType ResourceType) const
{
	if (ResourceInventory.Contains(ResourceType))
	{
		return ResourceInventory[ResourceType];
	}
	return 0;
}

bool UInventoryComponent::HasResource(EResourceType ResourceType, int32 MinQuantity) const
{
	return GetResourceQuantity(ResourceType) >= MinQuantity;
	// Check if adding this item would exceed weight limit
	float ItemWeight = Item->Weight * Quantity;
	if (CurrentWeight + ItemWeight > MaxWeight)
	{
		return false;
	}

	int32 RemainingQuantity = Quantity;

	// If item is stackable, try to stack it with existing items
	if (Item->bIsStackable)
	{
		int32 StackableSlot = FindStackableSlot(Item);
		while (StackableSlot != -1 && RemainingQuantity > 0)
		{
			FInventorySlot& Slot = InventorySlots[StackableSlot];
			int32 AvailableSpace = Item->MaxStackSize - Slot.Quantity;
			int32 AmountToAdd = FMath::Min(RemainingQuantity, AvailableSpace);
			
			Slot.Quantity += AmountToAdd;
			RemainingQuantity -= AmountToAdd;

			if (RemainingQuantity > 0)
			{
				StackableSlot = FindStackableSlot(Item);
			}
			else
			{
				break;
			}
		}
	}

	// Add remaining items to empty slots
	while (RemainingQuantity > 0)
	{
		int32 EmptySlot = FindEmptySlot();
		if (EmptySlot == -1)
		{
			// Inventory full, return false
			return false;
		}

		int32 AmountToAdd = Item->bIsStackable ? FMath::Min(RemainingQuantity, Item->MaxStackSize) : 1;
		InventorySlots[EmptySlot] = FInventorySlot(Item, AmountToAdd);
		RemainingQuantity -= AmountToAdd;

		if (!Item->bIsStackable)
		{
			break;
		}
	}

	UpdateWeight();
	OnInventoryChanged.Broadcast(this);
	return RemainingQuantity == 0;
}

bool UInventoryComponent::RemoveItem(UItem* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		return false;
	}

	if (!HasItem(Item, Quantity))
	{
		return false;
	}

	int32 RemainingQuantity = Quantity;

	for (int32 i = 0; i < InventorySlots.Num() && RemainingQuantity > 0; ++i)
	{
		if (InventorySlots[i].Item == Item)
		{
			int32 AmountToRemove = FMath::Min(RemainingQuantity, InventorySlots[i].Quantity);
			InventorySlots[i].Quantity -= AmountToRemove;
			RemainingQuantity -= AmountToRemove;

			if (InventorySlots[i].Quantity <= 0)
			{
				InventorySlots[i] = FInventorySlot();
			}
		}
	}

	UpdateWeight();
	OnInventoryChanged.Broadcast(this);
	return true;
}

bool UInventoryComponent::RemoveItemFromSlot(int32 SlotIndex, int32 Quantity)
{
	if (SlotIndex < 0 || SlotIndex >= InventorySlots.Num())
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (!Slot.Item || Slot.Quantity < Quantity)
	{
		return false;
	}

	Slot.Quantity -= Quantity;
	if (Slot.Quantity <= 0)
	{
		Slot = FInventorySlot();
	}

	UpdateWeight();
	OnInventoryChanged.Broadcast(this);
	return true;
}

FInventorySlot UInventoryComponent::GetItemAtSlot(int32 SlotIndex) const
{
	if (SlotIndex >= 0 && SlotIndex < InventorySlots.Num())
	{
		return InventorySlots[SlotIndex];
	}
	return FInventorySlot();
}

int32 UInventoryComponent::FindItem(UItem* Item) const
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].Item == Item)
		{
			return i;
		}
	}
	return -1;
}

bool UInventoryComponent::HasItem(UItem* Item, int32 Quantity) const
{
	return GetItemQuantity(Item) >= Quantity;
}

int32 UInventoryComponent::GetItemQuantity(UItem* Item) const
{
	int32 TotalQuantity = 0;
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == Item)
		{
			TotalQuantity += Slot.Quantity;
		}
	}
	return TotalQuantity;
}

void UInventoryComponent::SortInventory()
{
	// Sort by item ID, then by quantity
	InventorySlots.Sort([](const FInventorySlot& A, const FInventorySlot& B)
	{
		if (!A.Item && B.Item) return false;
		if (A.Item && !B.Item) return true;
		if (!A.Item && !B.Item) return false;

		if (A.Item->ItemID != B.Item->ItemID)
		{
			return A.Item->ItemID < B.Item->ItemID;
		}
		return A.Quantity > B.Quantity;
	});

	OnInventoryChanged.Broadcast(this);
}

int32 UInventoryComponent::GetEmptySlotCount() const
{
	int32 Count = 0;
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (!Slot.Item)
		{
			++Count;
		}
	}
	return Count;
}

bool UInventoryComponent::IsFull() const
{
	return GetEmptySlotCount() == 0;
}

float UInventoryComponent::GetCurrentWeight() const
{
	return CurrentWeight;
}

void UInventoryComponent::ClearInventory()
{
	ResourceInventory.Empty();
	InventorySlots.Empty();
	InventorySlots.SetNum(MaxSlots);
	CurrentWeight = 0.0f;
	OnInventoryChanged.Broadcast(this);
}

void UInventoryComponent::SaveInventoryState()
{
	// Stub for future SpacetimeDB integration
	// This would serialize the inventory and send it to SpacetimeDB
	FString SerializedData = SerializeInventory();
	// TODO: Send to SpacetimeDB when integrated
}

void UInventoryComponent::LoadInventoryState()
{
	// Stub for future SpacetimeDB integration
	// This would retrieve inventory data from SpacetimeDB and deserialize it
	// TODO: Retrieve from SpacetimeDB when integrated
	// FString SerializedData = GetFromSpacetimeDB();
	// DeserializeInventory(SerializedData);
}

FString UInventoryComponent::SerializeInventory() const
{
	// Simple JSON-like serialization for persistence
	FString Result = TEXT("{\"slots\":[");
	
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		const FInventorySlot& Slot = InventorySlots[i];
		if (Slot.Item)
		{
			Result += FString::Printf(TEXT("{\"itemID\":%d,\"quantity\":%d}"),
				Slot.Item->ItemID, Slot.Quantity);
		}
		else
		{
			Result += TEXT("{\"itemID\":-1,\"quantity\":0}");
		}

		if (i < InventorySlots.Num() - 1)
		{
			Result += TEXT(",");
		}
	}

	Result += TEXT("]}");
	return Result;
}

bool UInventoryComponent::DeserializeInventory(const FString& JsonString)
{
	// Stub for deserialization
	// In a full implementation, this would parse the JSON and recreate inventory state
	// TODO: Implement proper JSON parsing when item database is available
	return false;
}

void UInventoryComponent::UpdateWeight()
{
	CurrentWeight = 0.0f;
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item)
		{
			CurrentWeight += Slot.Item->Weight * Slot.Quantity;
		}
	}
}

int32 UInventoryComponent::FindEmptySlot() const
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (!InventorySlots[i].Item)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::FindStackableSlot(UItem* Item) const
{
	if (!Item || !Item->bIsStackable)
	{
		return -1;
	}

	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		const FInventorySlot& Slot = InventorySlots[i];
		if (Slot.Item == Item && Slot.Quantity < Item->MaxStackSize)
		{
			return i;
		}
	}
	return -1;
}
