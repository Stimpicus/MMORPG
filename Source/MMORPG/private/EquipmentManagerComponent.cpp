// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquipmentManagerComponent.h"
#include "PlayerAttributesComponent.h"
#include "PlayerSkillsComponent.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AttributesComponent = nullptr;
	SkillsComponent = nullptr;
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheComponentReferences();
}

void UEquipmentManagerComponent::CacheComponentReferences()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		AttributesComponent = Owner->FindComponentByClass<UPlayerAttributesComponent>();
		SkillsComponent = Owner->FindComponentByClass<UPlayerSkillsComponent>();
	}
}

void UEquipmentManagerComponent::ApplyEquipmentModifier(const FEquipmentModifier& Modifier)
{
	// Remove existing modifier with the same ID if it exists
	RemoveEquipmentModifier(Modifier.ModifierID);

	// Add the new modifier
	ActiveModifiers.Add(Modifier);

	// Recalculate all modifiers
	RecalculateModifiers();
}

void UEquipmentManagerComponent::RemoveEquipmentModifier(FName ModifierID)
{
	// Find and remove the modifier
	int32 IndexToRemove = -1;
	for (int32 i = 0; i < ActiveModifiers.Num(); ++i)
	{
		if (ActiveModifiers[i].ModifierID == ModifierID)
		{
			IndexToRemove = i;
			break;
		}
	}

	if (IndexToRemove >= 0)
	{
		ActiveModifiers.RemoveAt(IndexToRemove);
		RecalculateModifiers();
	}
}

void UEquipmentManagerComponent::ClearAllModifiers()
{
	ActiveModifiers.Empty();
	RecalculateModifiers();
}

void UEquipmentManagerComponent::RecalculateModifiers()
{
	// Ensure component references are valid
	if (!AttributesComponent || !SkillsComponent)
	{
		CacheComponentReferences();
	}

	// Reset all modifiers to zero
	if (AttributesComponent)
	{
		AttributesComponent->ApplyMaxHPModifier(0.0f);
		AttributesComponent->ApplyMaxManaModifier(0.0f);
		AttributesComponent->ApplyMaxStaminaModifier(0.0f);
	}

	if (SkillsComponent)
	{
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::Toughness);
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::ManaEfficiency);
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::StaminaEfficiency);
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::MeleeCombat);
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::RangedCombat);
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::MagicalAbility);
		SkillsComponent->RemoveSkillEquipmentModifier(ESkillType::ResourceGathering);
	}

	// Sum up all modifiers
	float TotalMaxHPModifier = 0.0f;
	float TotalMaxManaModifier = 0.0f;
	float TotalMaxStaminaModifier = 0.0f;
	TMap<ESkillType, float> TotalSkillModifiers;

	for (const FEquipmentModifier& Modifier : ActiveModifiers)
	{
		// Sum attribute modifiers
		for (const auto& AttributePair : Modifier.AttributeModifiers)
		{
			switch (AttributePair.Key)
			{
			case EAttributeType::MaxHP:
				TotalMaxHPModifier += AttributePair.Value;
				break;
			case EAttributeType::MaxMana:
				TotalMaxManaModifier += AttributePair.Value;
				break;
			case EAttributeType::MaxStamina:
				TotalMaxStaminaModifier += AttributePair.Value;
				break;
			}
		}

		// Sum skill modifiers
		for (const auto& SkillPair : Modifier.SkillModifiers)
		{
			if (!TotalSkillModifiers.Contains(SkillPair.Key))
			{
				TotalSkillModifiers.Add(SkillPair.Key, 0.0f);
			}
			TotalSkillModifiers[SkillPair.Key] += SkillPair.Value;
		}
	}

	// Apply accumulated modifiers
	if (AttributesComponent)
	{
		AttributesComponent->ApplyMaxHPModifier(TotalMaxHPModifier);
		AttributesComponent->ApplyMaxManaModifier(TotalMaxManaModifier);
		AttributesComponent->ApplyMaxStaminaModifier(TotalMaxStaminaModifier);
	}

	if (SkillsComponent)
	{
		for (const auto& SkillPair : TotalSkillModifiers)
		{
			SkillsComponent->ApplySkillEquipmentModifier(SkillPair.Key, SkillPair.Value);
		}
	}
}
