// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSkillsComponent.h"
#include "EquipmentModifier.generated.h"

/**
 * Enum for attribute types that can be modified by equipment
 */
UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	MaxHP				UMETA(DisplayName = "Max HP"),
	MaxMana				UMETA(DisplayName = "Max Mana"),
	MaxStamina			UMETA(DisplayName = "Max Stamina")
};

/**
 * Structure representing a single equipment modifier
 */
USTRUCT(BlueprintType)
struct FEquipmentModifier
{
	GENERATED_BODY()

	// Attribute modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Modifier")
	TMap<EAttributeType, float> AttributeModifiers;

	// Skill modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Modifier")
	TMap<ESkillType, float> SkillModifiers;

	// Unique identifier for this modifier (useful for tracking which equipment piece is providing it)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Modifier")
	FName ModifierID;

	FEquipmentModifier()
		: ModifierID(NAME_None)
	{
	}

	FEquipmentModifier(FName InModifierID)
		: ModifierID(InModifierID)
	{
	}
};
