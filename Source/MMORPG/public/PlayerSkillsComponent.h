// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentModifier.h"
#include "PlayerSkillsComponent.generated.h"

/**
 * Structure representing a single skill
 */
USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	// Current XP in this skill
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillXP;

	// Current skill level (calculated from XP)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SkillLevel;

	// Equipment-based modifier to skill effectiveness
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	float EquipmentModifier;

	FSkillData()
		: SkillXP(0.0f)
		, SkillLevel(1)
		, EquipmentModifier(0.0f)
	{
	}
};

/**
 * Component that manages player skills and progression.
 * Designed to be modular and support networking and persistence.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UPlayerSkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerSkillsComponent();

	virtual void BeginPlay() override;

	// Get skill level
	UFUNCTION(BlueprintPure, Category = "Skills")
	int32 GetSkillLevel(ESkillType SkillType) const;

	// Get skill XP
	UFUNCTION(BlueprintPure, Category = "Skills")
	float GetSkillXP(ESkillType SkillType) const;

	// Get effective skill value (level + equipment modifier)
	UFUNCTION(BlueprintPure, Category = "Skills")
	float GetEffectiveSkillValue(ESkillType SkillType) const;

	// Add XP to a skill
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void AddSkillXP(ESkillType SkillType, float Amount);

	// Apply equipment modifier to a skill
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void ApplySkillEquipmentModifier(ESkillType SkillType, float Modifier);

	// Remove equipment modifier from a skill
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void RemoveSkillEquipmentModifier(ESkillType SkillType);

	// Calculate level from XP (can be overridden for custom progression curves)
	UFUNCTION(BlueprintPure, Category = "Skills")
	int32 CalculateLevelFromXP(float XP) const;

	// Calculate XP required for next level
	UFUNCTION(BlueprintPure, Category = "Skills")
	float GetXPRequiredForNextLevel(int32 CurrentLevel) const;

protected:
	// Skill data for each skill type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Skills")
	TMap<ESkillType, FSkillData> Skills;

	// XP multiplier per level (affects progression curve)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills|Progression")
	float XPMultiplierPerLevel;

	// Base XP required for level 2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills|Progression")
	float BaseXPForLevel2;

	// Update skill level based on current XP
	void UpdateSkillLevel(ESkillType SkillType);

	// Networking support
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Initialize all skills with default values
	void InitializeSkills();
};
