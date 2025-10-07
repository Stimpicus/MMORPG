// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "EquipmentItem.generated.h"

/**
 * Enumeration for equipment slots
 */
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None		UMETA(DisplayName = "None"),
	Head		UMETA(DisplayName = "Head"),
	Torso		UMETA(DisplayName = "Torso"),
	Arms		UMETA(DisplayName = "Arms"),
	Hands		UMETA(DisplayName = "Hands"),
	LeftHand	UMETA(DisplayName = "Left Hand"),
	RightHand	UMETA(DisplayName = "Right Hand"),
	Legs		UMETA(DisplayName = "Legs"),
	Feet		UMETA(DisplayName = "Feet")
};

/**
 * Structure for attribute modifiers
 */
USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	FString AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float ModifierValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	bool bIsPercentage;

	FAttributeModifier()
		: AttributeName(TEXT(""))
		, ModifierValue(0.0f)
		, bIsPercentage(false)
	{
	}
};

/**
 * Equipment item class that can be equipped to specific slots
 * Provides attribute and skill modifiers
 */
UCLASS(Blueprintable, BlueprintType)
class MMORPG_API UEquipmentItem : public UItem
{
	GENERATED_BODY()

public:
	UEquipmentItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	EEquipmentSlot EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TArray<FAttributeModifier> AttributeModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TArray<FAttributeModifier> SkillModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 RequiredLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 ArmorRating;

	// Apply modifiers when equipped
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ApplyModifiers();

	// Remove modifiers when unequipped
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void RemoveModifiers();

	virtual void Use() override;
};
