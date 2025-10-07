// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterDisplayWidget.h"

void UCharacterDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize with placeholder data
	RefreshCharacterData();
}

void UCharacterDisplayWidget::RefreshCharacterData()
{
	// Placeholder: In the future, this will fetch data from a character manager/component
	// For now, the placeholder values in the header are used
}

float UCharacterDisplayWidget::GetHealthPercentage() const
{
	if (MaxHealth <= 0)
	{
		return 0.0f;
	}
	return static_cast<float>(Health) / static_cast<float>(MaxHealth);
}

float UCharacterDisplayWidget::GetManaPercentage() const
{
	if (MaxMana <= 0)
	{
		return 0.0f;
	}
	return static_cast<float>(Mana) / static_cast<float>(MaxMana);
}

float UCharacterDisplayWidget::GetExperiencePercentage() const
{
	if (ExperienceToNextLevel <= 0)
	{
		return 0.0f;
	}
	return static_cast<float>(Experience) / static_cast<float>(ExperienceToNextLevel);
}
