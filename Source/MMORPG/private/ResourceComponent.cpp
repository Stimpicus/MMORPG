// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceComponent.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default values
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MaxMana = 100.0f;
	CurrentMana = MaxMana;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize current values to max
	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
	CurrentStamina = MaxStamina;
}

float UResourceComponent::GetCurrentResource(EResourceType ResourceType) const
{
	const float* ResourceValue = GetResourceRef(ResourceType);
	return ResourceValue ? *ResourceValue : 0.0f;
}

float UResourceComponent::GetMaxResource(EResourceType ResourceType) const
{
	const float* MaxValue = GetMaxResourceRef(ResourceType);
	return MaxValue ? *MaxValue : 0.0f;
}

bool UResourceComponent::ConsumeResource(EResourceType ResourceType, float Amount)
{
	if (!HasEnoughResource(ResourceType, Amount))
	{
		return false;
	}

	float* ResourceValue = GetResourceRef(ResourceType);
	if (ResourceValue)
	{
		*ResourceValue = FMath::Max(0.0f, *ResourceValue - Amount);
		OnResourceChanged.Broadcast(ResourceType, *ResourceValue, GetMaxResource(ResourceType));
		return true;
	}

	return false;
}

void UResourceComponent::RestoreResource(EResourceType ResourceType, float Amount)
{
	float* ResourceValue = GetResourceRef(ResourceType);
	float MaxValue = GetMaxResource(ResourceType);

	if (ResourceValue)
	{
		*ResourceValue = FMath::Min(MaxValue, *ResourceValue + Amount);
		OnResourceChanged.Broadcast(ResourceType, *ResourceValue, MaxValue);
	}
}

bool UResourceComponent::HasEnoughResource(EResourceType ResourceType, float Amount) const
{
	return GetCurrentResource(ResourceType) >= Amount;
}

bool UResourceComponent::HasEnoughResources(const TArray<FResourceCost>& Costs) const
{
	for (const FResourceCost& Cost : Costs)
	{
		if (!HasEnoughResource(Cost.ResourceType, Cost.Amount))
		{
			return false;
		}
	}
	return true;
}

bool UResourceComponent::ConsumeResources(const TArray<FResourceCost>& Costs)
{
	// First check if all resources are available
	if (!HasEnoughResources(Costs))
	{
		return false;
	}

	// Consume all resources
	for (const FResourceCost& Cost : Costs)
	{
		ConsumeResource(Cost.ResourceType, Cost.Amount);
	}

	return true;
}

float* UResourceComponent::GetResourceRef(EResourceType ResourceType)
{
	switch (ResourceType)
	{
	case EResourceType::Health:
		return &CurrentHealth;
	case EResourceType::Mana:
		return &CurrentMana;
	case EResourceType::Stamina:
		return &CurrentStamina;
	default:
		return nullptr;
	}
}

const float* UResourceComponent::GetResourceRef(EResourceType ResourceType) const
{
	switch (ResourceType)
	{
	case EResourceType::Health:
		return &CurrentHealth;
	case EResourceType::Mana:
		return &CurrentMana;
	case EResourceType::Stamina:
		return &CurrentStamina;
	default:
		return nullptr;
	}
}

float* UResourceComponent::GetMaxResourceRef(EResourceType ResourceType)
{
	switch (ResourceType)
	{
	case EResourceType::Health:
		return &MaxHealth;
	case EResourceType::Mana:
		return &MaxMana;
	case EResourceType::Stamina:
		return &MaxStamina;
	default:
		return nullptr;
	}
}

const float* UResourceComponent::GetMaxResourceRef(EResourceType ResourceType) const
{
	switch (ResourceType)
	{
	case EResourceType::Health:
		return &MaxHealth;
	case EResourceType::Mana:
		return &MaxMana;
	case EResourceType::Stamina:
		return &MaxStamina;
	default:
		return nullptr;
	}
}
