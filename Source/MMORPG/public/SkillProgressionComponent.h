// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceTypes.h"
#include "SkillProgressionComponent.generated.h"

/**
 * Structure representing skill data for a gathering skill
 */
USTRUCT(BlueprintType)
struct FGatheringSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EGatheringSkillType SkillType = EGatheringSkillType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 CurrentExperience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 CurrentLevel = 1;

	FGatheringSkillData()
		: SkillType(EGatheringSkillType::None), CurrentExperience(0), CurrentLevel(1)
	{
	}

	FGatheringSkillData(EGatheringSkillType Type)
		: SkillType(Type), CurrentExperience(0), CurrentLevel(1)
	{
	}
};

/**
 * Component that manages skill progression for gathering activities
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API USkillProgressionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillProgressionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	/**
	 * Map of skill types to their data
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Progression")
	TMap<EGatheringSkillType, FGatheringSkillData> SkillData;

	/**
	 * Experience required for the first level
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Progression")
	int32 BaseExperienceForLevel = 100;

	/**
	 * Multiplier applied to experience requirements for each subsequent level
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Progression")
	float ExperienceMultiplierPerLevel = 1.5f;

	/**
	 * Add experience to a gathering skill
	 * @param SkillType - The skill to add experience to
	 * @param Experience - The amount of experience to add
	 * @return True if a level up occurred
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Progression")
	bool AddSkillExperience(EGatheringSkillType SkillType, int32 Experience);

	/**
	 * Get the current level of a skill
	 * @param SkillType - The skill to query
	 * @return The current level of the skill
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skill Progression")
	int32 GetSkillLevel(EGatheringSkillType SkillType) const;

	/**
	 * Get the current experience of a skill
	 * @param SkillType - The skill to query
	 * @return The current experience of the skill
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skill Progression")
	int32 GetSkillExperience(EGatheringSkillType SkillType) const;

	/**
	 * Calculate the experience required for a specific level
	 * @param Level - The level to calculate experience for
	 * @return The total experience required to reach that level
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skill Progression")
	int32 CalculateExperienceForLevel(int32 Level) const;

	/**
	 * Get the experience required to reach the next level for a skill
	 * @param SkillType - The skill to query
	 * @return The experience required for the next level
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skill Progression")
	int32 GetExperienceToNextLevel(EGatheringSkillType SkillType) const;

	/**
	 * Initialize a skill if it doesn't exist
	 * @param SkillType - The skill to initialize
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Progression")
	void InitializeSkill(EGatheringSkillType SkillType);

	/**
	 * Get skill data for a specific skill type
	 * @param SkillType - The skill to query
	 * @param OutSkillData - Output parameter with the skill data
	 * @return True if the skill exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Progression")
	bool GetSkillData(EGatheringSkillType SkillType, FGatheringSkillData& OutSkillData) const;

private:
	/**
	 * Check if a skill should level up and perform the level up
	 * @param SkillType - The skill to check
	 * @return True if a level up occurred
	 */
	bool CheckAndPerformLevelUp(EGatheringSkillType SkillType);
};
