// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDisplayWidget.generated.h"

/**
 * Skill data structure for displaying skill progression
 */
USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FString SkillName = TEXT("Unknown Skill");

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	int32 CurrentLevel = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	int32 MaxLevel = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	int32 CurrentExperience = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	int32 ExperienceToNextLevel = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	FString Description = TEXT("A skill");
};

/**
 * Skill Display Panel Widget
 * Shows all character skills with values and progression
 */
UCLASS()
class MMORPG_API USkillDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Combat Skills
	UPROPERTY(BlueprintReadWrite, Category = "Skills|Combat")
	FSkillData SwordSkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Combat")
	FSkillData ArcherySkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Combat")
	FSkillData MagicSkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Combat")
	FSkillData DefenseSkill;

	// Crafting Skills
	UPROPERTY(BlueprintReadWrite, Category = "Skills|Crafting")
	FSkillData BlacksmithingSkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Crafting")
	FSkillData AlchemySkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Crafting")
	FSkillData EnchantingSkill;

	// Gathering Skills
	UPROPERTY(BlueprintReadWrite, Category = "Skills|Gathering")
	FSkillData MiningSkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Gathering")
	FSkillData HerbalismSkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Gathering")
	FSkillData FishingSkill;

	// Utility Skills
	UPROPERTY(BlueprintReadWrite, Category = "Skills|Utility")
	FSkillData StealthSkill;

	UPROPERTY(BlueprintReadWrite, Category = "Skills|Utility")
	FSkillData LockpickingSkill;

	// All skills array for easy iteration
	UPROPERTY(BlueprintReadWrite, Category = "Skills")
	TArray<FSkillData> AllSkills;

	// Blueprint-callable functions
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void RefreshSkills();

	UFUNCTION(BlueprintCallable, Category = "Skills")
	FSkillData GetSkillByName(const FString& SkillName) const;

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void AddSkillExperience(const FString& SkillName, int32 ExperienceAmount);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	float GetSkillProgressPercentage(const FSkillData& Skill) const;

	UFUNCTION(BlueprintCallable, Category = "Skills")
	int32 GetTotalSkillLevel() const;

protected:
	virtual void NativeConstruct() override;

private:
	void InitializePlaceholderSkills();
	void PopulateSkillsArray();
};
