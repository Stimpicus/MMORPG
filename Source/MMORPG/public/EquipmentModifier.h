// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentModifier.generated.h"

/**
 * Enum defining all skill types available in the game
 */
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Toughness			UMETA(DisplayName = "Toughness"),
	ManaEfficiency		UMETA(DisplayName = "Mana Efficiency"),
	StaminaEfficiency	UMETA(DisplayName = "Stamina Efficiency"),
	MeleeCombat			UMETA(DisplayName = "Melee Combat"),
	RangedCombat		UMETA(DisplayName = "Ranged Combat"),
	MagicalAbility		UMETA(DisplayName = "Magical Ability"),
	ResourceGathering	UMETA(DisplayName = "Resource Gathering")
};

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
