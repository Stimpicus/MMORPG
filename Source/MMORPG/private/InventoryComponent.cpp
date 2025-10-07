// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UInventoryComponent::AddResource(EResourceType ResourceType, int32 Quantity)
{
	if (ResourceType == EResourceType::None || Quantity <= 0)
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
}

void UInventoryComponent::ClearInventory()
{
	ResourceInventory.Empty();
}
