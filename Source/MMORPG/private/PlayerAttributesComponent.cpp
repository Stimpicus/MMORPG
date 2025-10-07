// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerAttributesComponent.h"
#include "Net/UnrealNetwork.h"

UPlayerAttributesComponent::UPlayerAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedComponent(true);

	// Initialize default values
	CurrentXP = 0.0f;
	Credits = 0;

	// Base max values
	BaseMaxHP = 100.0f;
	BaseMaxMana = 100.0f;
	BaseMaxStamina = 100.0f;

	// Current values start at max
	CurrentHP = BaseMaxHP;
	CurrentMana = BaseMaxMana;
	CurrentStamina = BaseMaxStamina;

	// Initialize equipment modifiers
	EquipmentMaxHPModifier = 0.0f;
	EquipmentMaxManaModifier = 0.0f;
	EquipmentMaxStaminaModifier = 0.0f;

	// Calculate initial max values
	MaxHP = BaseMaxHP;
	MaxMana = BaseMaxMana;
	MaxStamina = BaseMaxStamina;
}

void UPlayerAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateMaxValues();
}

void UPlayerAttributesComponent::AddXP(float Amount)
{
	if (Amount > 0.0f)
	{
		CurrentXP += Amount;
	}
}

void UPlayerAttributesComponent::ModifyHP(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);
}

void UPlayerAttributesComponent::ModifyMana(float Amount)
{
	CurrentMana = FMath::Clamp(CurrentMana + Amount, 0.0f, MaxMana);
}

void UPlayerAttributesComponent::ModifyStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0.0f, MaxStamina);
}

void UPlayerAttributesComponent::AddCredits(int32 Amount)
{
	if (Amount > 0)
	{
		Credits += Amount;
	}
}

bool UPlayerAttributesComponent::SpendCredits(int32 Amount)
{
	if (Amount > 0 && Credits >= Amount)
	{
		Credits -= Amount;
		return true;
	}
	return false;
}

void UPlayerAttributesComponent::ApplyMaxHPModifier(float Modifier)
{
	EquipmentMaxHPModifier = Modifier;
	RecalculateMaxValues();
}

void UPlayerAttributesComponent::ApplyMaxManaModifier(float Modifier)
{
	EquipmentMaxManaModifier = Modifier;
	RecalculateMaxValues();
}

void UPlayerAttributesComponent::ApplyMaxStaminaModifier(float Modifier)
{
	EquipmentMaxStaminaModifier = Modifier;
	RecalculateMaxValues();
}

void UPlayerAttributesComponent::RecalculateMaxValues()
{
	MaxHP = BaseMaxHP + EquipmentMaxHPModifier;
	MaxMana = BaseMaxMana + EquipmentMaxManaModifier;
	MaxStamina = BaseMaxStamina + EquipmentMaxStaminaModifier;

	// Clamp current values to new max values
	CurrentHP = FMath::Min(CurrentHP, MaxHP);
	CurrentMana = FMath::Min(CurrentMana, MaxMana);
	CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);
}

void UPlayerAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAttributesComponent, CurrentXP);
	DOREPLIFETIME(UPlayerAttributesComponent, CurrentHP);
	DOREPLIFETIME(UPlayerAttributesComponent, CurrentMana);
	DOREPLIFETIME(UPlayerAttributesComponent, CurrentStamina);
	DOREPLIFETIME(UPlayerAttributesComponent, Credits);
}
