// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquipmentItem.h"

UEquipmentItem::UEquipmentItem()
{
	EquipmentSlot = EEquipmentSlot::None;
	RequiredLevel = 1;
	ArmorRating = 0;
	bIsStackable = false;
	MaxStackSize = 1;
}

void UEquipmentItem::ApplyModifiers()
{
	// Hook for applying attribute and skill modifiers
	// This will be called when the item is equipped
	// Implementation will be done when character/attribute system is available
}

void UEquipmentItem::RemoveModifiers()
{
	// Hook for removing attribute and skill modifiers
	// This will be called when the item is unequipped
	// Implementation will be done when character/attribute system is available
}

void UEquipmentItem::Use()
{
	// Equipment items are equipped rather than used
	// This could trigger equip logic
}
