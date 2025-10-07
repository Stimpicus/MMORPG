// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentModifier.h"
#include "EquipmentManagerComponent.generated.h"

// Forward declarations
class UPlayerAttributesComponent;
class UPlayerSkillsComponent;

/**
 * Component that manages equipment and applies modifiers to attributes and skills.
 * This component coordinates between equipment items and the player's attributes/skills.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent();

	virtual void BeginPlay() override;

	// Apply an equipment modifier
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ApplyEquipmentModifier(const FEquipmentModifier& Modifier);

	// Remove an equipment modifier by ID
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void RemoveEquipmentModifier(FName ModifierID);

	// Clear all equipment modifiers
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ClearAllModifiers();

	// Get all active modifiers
	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<FEquipmentModifier> GetActiveModifiers() const { return ActiveModifiers; }

protected:
	// Currently active equipment modifiers
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	TArray<FEquipmentModifier> ActiveModifiers;

	// Cached component references
	UPROPERTY()
	UPlayerAttributesComponent* AttributesComponent;

	UPROPERTY()
	UPlayerSkillsComponent* SkillsComponent;

	// Recalculate and apply all modifiers
	void RecalculateModifiers();

	// Cache component references
	void CacheComponentReferences();
};
