// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMORPGCharacter.generated.h"

// Forward declarations
class UPlayerAttributesComponent;
class UPlayerSkillsComponent;
class UEquipmentManagerComponent;

/**
 * Example character class demonstrating the use of the skills and attributes system.
 * This is a modular character class that can be extended for specific player character types.
 */
UCLASS()
class MMORPG_API AMMORPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMMORPGCharacter();

protected:
	virtual void BeginPlay() override;

	// Player attributes component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerAttributesComponent* AttributesComponent;

	// Player skills component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerSkillsComponent* SkillsComponent;

	// Equipment manager component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEquipmentManagerComponent* EquipmentManagerComponent;

public:
	// Getters for components (Blueprint accessible)
	UFUNCTION(BlueprintPure, Category = "Character|Components")
	UPlayerAttributesComponent* GetAttributesComponent() const { return AttributesComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|Components")
	UPlayerSkillsComponent* GetSkillsComponent() const { return SkillsComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|Components")
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const { return EquipmentManagerComponent; }
};
