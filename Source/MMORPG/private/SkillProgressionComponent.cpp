// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillProgressionComponent.h"

USkillProgressionComponent::USkillProgressionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillProgressionComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USkillProgressionComponent::AddSkillExperience(EGatheringSkillType SkillType, int32 Experience)
{
	if (SkillType == EGatheringSkillType::None || Experience <= 0)
	{
		return false;
	}

	// Initialize skill if it doesn't exist
	if (!SkillData.Contains(SkillType))
	{
		InitializeSkill(SkillType);
	}

	// Add experience
	SkillData[SkillType].CurrentExperience += Experience;

	// Check for level up
	return CheckAndPerformLevelUp(SkillType);
}

int32 USkillProgressionComponent::GetSkillLevel(EGatheringSkillType SkillType) const
{
	if (SkillData.Contains(SkillType))
	{
		return SkillData[SkillType].CurrentLevel;
	}
	return 1; // Default level
}

int32 USkillProgressionComponent::GetSkillExperience(EGatheringSkillType SkillType) const
{
	if (SkillData.Contains(SkillType))
	{
		return SkillData[SkillType].CurrentExperience;
	}
	return 0;
}

int32 USkillProgressionComponent::CalculateExperienceForLevel(int32 Level) const
{
	if (Level <= 1)
	{
		return 0;
	}

	int32 TotalExperience = 0;
	for (int32 i = 1; i < Level; ++i)
	{
		TotalExperience += FMath::RoundToInt(BaseExperienceForLevel * FMath::Pow(ExperienceMultiplierPerLevel, i - 1));
	}

	return TotalExperience;
}

int32 USkillProgressionComponent::GetExperienceToNextLevel(EGatheringSkillType SkillType) const
{
	if (!SkillData.Contains(SkillType))
	{
		return BaseExperienceForLevel;
	}

	const FGatheringSkillData& Data = SkillData[SkillType];
	int32 ExperienceForNextLevel = CalculateExperienceForLevel(Data.CurrentLevel + 1);
	int32 RemainingExperience = ExperienceForNextLevel - Data.CurrentExperience;

	return FMath::Max(0, RemainingExperience);
}

void USkillProgressionComponent::InitializeSkill(EGatheringSkillType SkillType)
{
	if (SkillType != EGatheringSkillType::None && !SkillData.Contains(SkillType))
	{
		FGatheringSkillData NewSkillData(SkillType);
		SkillData.Add(SkillType, NewSkillData);
	}
}

bool USkillProgressionComponent::GetSkillData(EGatheringSkillType SkillType, FGatheringSkillData& OutSkillData) const
{
	if (SkillData.Contains(SkillType))
	{
		OutSkillData = SkillData[SkillType];
		return true;
	}
	return false;
}

bool USkillProgressionComponent::CheckAndPerformLevelUp(EGatheringSkillType SkillType)
{
	if (!SkillData.Contains(SkillType))
	{
		return false;
	}

	FGatheringSkillData& Data = SkillData[SkillType];
	int32 ExperienceForNextLevel = CalculateExperienceForLevel(Data.CurrentLevel + 1);

	bool LeveledUp = false;

	// Check if we can level up (potentially multiple times)
	while (Data.CurrentExperience >= ExperienceForNextLevel)
	{
		Data.CurrentLevel++;
		ExperienceForNextLevel = CalculateExperienceForLevel(Data.CurrentLevel + 1);
		LeveledUp = true;
	}

	return LeveledUp;
}
