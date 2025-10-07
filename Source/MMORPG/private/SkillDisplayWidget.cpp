// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillDisplayWidget.h"

void USkillDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize with placeholder skills
	InitializePlaceholderSkills();
	PopulateSkillsArray();
}

void USkillDisplayWidget::InitializePlaceholderSkills()
{
	// Combat Skills
	SwordSkill.SkillName = TEXT("Sword Fighting");
	SwordSkill.CurrentLevel = 15;
	SwordSkill.CurrentExperience = 250;
	SwordSkill.ExperienceToNextLevel = 500;
	SwordSkill.Description = TEXT("Proficiency with swords and bladed weapons");

	ArcherySkill.SkillName = TEXT("Archery");
	ArcherySkill.CurrentLevel = 10;
	ArcherySkill.CurrentExperience = 100;
	ArcherySkill.ExperienceToNextLevel = 400;
	ArcherySkill.Description = TEXT("Skill with bows and ranged weapons");

	MagicSkill.SkillName = TEXT("Magic");
	MagicSkill.CurrentLevel = 20;
	MagicSkill.CurrentExperience = 400;
	MagicSkill.ExperienceToNextLevel = 600;
	MagicSkill.Description = TEXT("Ability to cast spells and manipulate mana");

	DefenseSkill.SkillName = TEXT("Defense");
	DefenseSkill.CurrentLevel = 12;
	DefenseSkill.CurrentExperience = 150;
	DefenseSkill.ExperienceToNextLevel = 450;
	DefenseSkill.Description = TEXT("Skill at blocking and avoiding damage");

	// Crafting Skills
	BlacksmithingSkill.SkillName = TEXT("Blacksmithing");
	BlacksmithingSkill.CurrentLevel = 8;
	BlacksmithingSkill.CurrentExperience = 50;
	BlacksmithingSkill.ExperienceToNextLevel = 350;
	BlacksmithingSkill.Description = TEXT("Ability to forge weapons and armor");

	AlchemySkill.SkillName = TEXT("Alchemy");
	AlchemySkill.CurrentLevel = 5;
	AlchemySkill.CurrentExperience = 20;
	AlchemySkill.ExperienceToNextLevel = 250;
	AlchemySkill.Description = TEXT("Knowledge of potions and elixirs");

	EnchantingSkill.SkillName = TEXT("Enchanting");
	EnchantingSkill.CurrentLevel = 3;
	EnchantingSkill.CurrentExperience = 10;
	EnchantingSkill.ExperienceToNextLevel = 200;
	EnchantingSkill.Description = TEXT("Ability to imbue items with magical properties");

	// Gathering Skills
	MiningSkill.SkillName = TEXT("Mining");
	MiningSkill.CurrentLevel = 18;
	MiningSkill.CurrentExperience = 300;
	MiningSkill.ExperienceToNextLevel = 550;
	MiningSkill.Description = TEXT("Proficiency in extracting ores and minerals");

	HerbalismSkill.SkillName = TEXT("Herbalism");
	HerbalismSkill.CurrentLevel = 7;
	HerbalismSkill.CurrentExperience = 80;
	HerbalismSkill.ExperienceToNextLevel = 300;
	HerbalismSkill.Description = TEXT("Knowledge of plants and herbs");

	FishingSkill.SkillName = TEXT("Fishing");
	FishingSkill.CurrentLevel = 4;
	FishingSkill.CurrentExperience = 30;
	FishingSkill.ExperienceToNextLevel = 220;
	FishingSkill.Description = TEXT("Ability to catch fish");

	// Utility Skills
	StealthSkill.SkillName = TEXT("Stealth");
	StealthSkill.CurrentLevel = 6;
	StealthSkill.CurrentExperience = 45;
	StealthSkill.ExperienceToNextLevel = 280;
	StealthSkill.Description = TEXT("Ability to move unseen");

	LockpickingSkill.SkillName = TEXT("Lockpicking");
	LockpickingSkill.CurrentLevel = 2;
	LockpickingSkill.CurrentExperience = 5;
	LockpickingSkill.ExperienceToNextLevel = 150;
	LockpickingSkill.Description = TEXT("Skill at picking locks and disarming traps");
}

void USkillDisplayWidget::PopulateSkillsArray()
{
	AllSkills.Empty();
	AllSkills.Add(SwordSkill);
	AllSkills.Add(ArcherySkill);
	AllSkills.Add(MagicSkill);
	AllSkills.Add(DefenseSkill);
	AllSkills.Add(BlacksmithingSkill);
	AllSkills.Add(AlchemySkill);
	AllSkills.Add(EnchantingSkill);
	AllSkills.Add(MiningSkill);
	AllSkills.Add(HerbalismSkill);
	AllSkills.Add(FishingSkill);
	AllSkills.Add(StealthSkill);
	AllSkills.Add(LockpickingSkill);
}

void USkillDisplayWidget::RefreshSkills()
{
	// Placeholder: In the future, this will fetch data from skill manager
	PopulateSkillsArray();
}

FSkillData USkillDisplayWidget::GetSkillByName(const FString& SkillName) const
{
	for (const FSkillData& Skill : AllSkills)
	{
		if (Skill.SkillName.Equals(SkillName))
		{
			return Skill;
		}
	}
	return FSkillData();
}

void USkillDisplayWidget::AddSkillExperience(const FString& SkillName, int32 ExperienceAmount)
{
	// Placeholder implementation
	// In the future, this will update the actual skill data
	for (FSkillData& Skill : AllSkills)
	{
		if (Skill.SkillName.Equals(SkillName))
		{
			Skill.CurrentExperience += ExperienceAmount;
			
			// Level up if enough experience
			while (Skill.CurrentExperience >= Skill.ExperienceToNextLevel && Skill.CurrentLevel < Skill.MaxLevel)
			{
				Skill.CurrentExperience -= Skill.ExperienceToNextLevel;
				Skill.CurrentLevel++;
				Skill.ExperienceToNextLevel = static_cast<int32>(Skill.ExperienceToNextLevel * 1.1f); // 10% increase
			}
			break;
		}
	}
}

float USkillDisplayWidget::GetSkillProgressPercentage(const FSkillData& Skill) const
{
	if (Skill.ExperienceToNextLevel <= 0)
	{
		return 1.0f;
	}
	return static_cast<float>(Skill.CurrentExperience) / static_cast<float>(Skill.ExperienceToNextLevel);
}

int32 USkillDisplayWidget::GetTotalSkillLevel() const
{
	int32 TotalLevel = 0;
	for (const FSkillData& Skill : AllSkills)
	{
		TotalLevel += Skill.CurrentLevel;
	}
	return TotalLevel;
}
