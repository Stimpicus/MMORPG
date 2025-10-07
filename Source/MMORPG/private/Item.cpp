// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item.h"

UItem::UItem()
{
	ItemName = TEXT("Default Item");
	Description = TEXT("A basic item");
	ItemID = 0;
	Weight = 0.0f;
	Value = 0;
	MaxStackSize = 1;
	CurrentStackSize = 1;
	bIsStackable = false;
}

void UItem::Use()
{
	// Base implementation - override in derived classes
}

FString UItem::GetItemInfo() const
{
	return FString::Printf(TEXT("Name: %s\nDescription: %s\nWeight: %.2f\nValue: %d"),
		*ItemName, *Description, Weight, Value);
}
