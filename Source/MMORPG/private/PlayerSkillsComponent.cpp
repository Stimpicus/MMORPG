// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerSkillsComponent.h"
#include "Net/UnrealNetwork.h"

UPlayerSkillsComponent::UPlayerSkillsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedComponent(true);

	// Default progression settings
	XPMultiplierPerLevel = 1.5f;
	BaseXPForLevel2 = 100.0f;
}

void UPlayerSkillsComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeSkills();
}

void UPlayerSkillsComponent::InitializeSkills()
{
	// Initialize all skill types if they don't exist
	TArray<ESkillType> AllSkills = {
		ESkillType::Toughness,
		ESkillType::ManaEfficiency,
		ESkillType::StaminaEfficiency,
		ESkillType::MeleeCombat,
		ESkillType::RangedCombat,
		ESkillType::MagicalAbility,
		ESkillType::ResourceGathering
	};

	for (ESkillType SkillType : AllSkills)
	{
		if (!Skills.Contains(SkillType))
		{
			FSkillData NewSkill;
			Skills.Add(SkillType, NewSkill);
		}
	}
}

int32 UPlayerSkillsComponent::GetSkillLevel(ESkillType SkillType) const
{
	const FSkillData* SkillData = Skills.Find(SkillType);
	return SkillData ? SkillData->SkillLevel : 1;
}

float UPlayerSkillsComponent::GetSkillXP(ESkillType SkillType) const
{
	const FSkillData* SkillData = Skills.Find(SkillType);
	return SkillData ? SkillData->SkillXP : 0.0f;
}

float UPlayerSkillsComponent::GetEffectiveSkillValue(ESkillType SkillType) const
{
	const FSkillData* SkillData = Skills.Find(SkillType);
	if (SkillData)
	{
		return static_cast<float>(SkillData->SkillLevel) + SkillData->EquipmentModifier;
	}
	return 1.0f;
}

void UPlayerSkillsComponent::AddSkillXP(ESkillType SkillType, float Amount)
{
	if (Amount <= 0.0f)
	{
		return;
	}

	FSkillData* SkillData = Skills.Find(SkillType);
	if (SkillData)
	{
		SkillData->SkillXP += Amount;
		UpdateSkillLevel(SkillType);
	}
}

void UPlayerSkillsComponent::ApplySkillEquipmentModifier(ESkillType SkillType, float Modifier)
{
	FSkillData* SkillData = Skills.Find(SkillType);
	if (SkillData)
	{
		SkillData->EquipmentModifier = Modifier;
	}
}

void UPlayerSkillsComponent::RemoveSkillEquipmentModifier(ESkillType SkillType)
{
	FSkillData* SkillData = Skills.Find(SkillType);
	if (SkillData)
	{
		SkillData->EquipmentModifier = 0.0f;
	}
}

int32 UPlayerSkillsComponent::CalculateLevelFromXP(float XP) const
{
	if (XP < BaseXPForLevel2)
	{
		return 1;
	}

	int32 Level = 1;
	float XPRequired = BaseXPForLevel2;
	float AccumulatedXP = 0.0f;

	while (AccumulatedXP + XPRequired <= XP)
	{
		AccumulatedXP += XPRequired;
		Level++;
		XPRequired = BaseXPForLevel2 * FMath::Pow(XPMultiplierPerLevel, Level - 1);
	}

	return Level;
}

float UPlayerSkillsComponent::GetXPRequiredForNextLevel(int32 CurrentLevel) const
{
	if (CurrentLevel < 1)
	{
		return BaseXPForLevel2;
	}

	return BaseXPForLevel2 * FMath::Pow(XPMultiplierPerLevel, CurrentLevel);
}

void UPlayerSkillsComponent::UpdateSkillLevel(ESkillType SkillType)
{
	FSkillData* SkillData = Skills.Find(SkillType);
	if (SkillData)
	{
		int32 NewLevel = CalculateLevelFromXP(SkillData->SkillXP);
		if (NewLevel != SkillData->SkillLevel)
		{
			SkillData->SkillLevel = NewLevel;
		}
	}
}

void UPlayerSkillsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerSkillsComponent, Skills);
}
