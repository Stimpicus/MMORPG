// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatCharacter.h"
#include "ResourceComponent.h"
#include "CombatComponent.h"
#include "CombatTypes.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	// Set default resource values
	if (ResourceComponent)
	{
		// These can be overridden in Blueprints or child classes
	}

	CurrentTarget = nullptr;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind to combat events
	if (CombatComponent)
	{
		CombatComponent->OnDamageTaken.AddDynamic(this, &ACombatCharacter::OnDamageTaken);
		CombatComponent->OnAttackExecuted.AddDynamic(this, &ACombatCharacter::OnAttackExecuted);
	}

	// Bind to resource events
	if (ResourceComponent)
	{
		ResourceComponent->OnResourceChanged.AddDynamic(this, &ACombatCharacter::OnResourceChanged);
	}
}

void ACombatCharacter::PerformMeleeAttack()
{
	if (CombatComponent && CurrentTarget)
	{
		CombatComponent->ExecuteMeleeAttack(CurrentTarget);
	}
}

void ACombatCharacter::PerformRangedAttack()
{
	if (CombatComponent && CurrentTarget)
	{
		CombatComponent->ExecuteRangedAttack(CurrentTarget);
	}
}

void ACombatCharacter::PerformMagicalAttack()
{
	if (CombatComponent && CurrentTarget)
	{
		CombatComponent->ExecuteMagicalAttack(CurrentTarget);
	}
}

void ACombatCharacter::EquipWeapon(const FWeaponData& Weapon)
{
	if (CombatComponent)
	{
		CombatComponent->SetEquippedWeapon(Weapon);
	}
}

void ACombatCharacter::OnDamageTaken(AActor* Instigator, float Damage)
{
	// Log damage for debugging
	UE_LOG(LogTemp, Warning, TEXT("%s took %.2f damage from %s"), 
		*GetName(), 
		Damage, 
		Instigator ? *Instigator->GetName() : TEXT("Unknown"));

	// Here you could:
	// - Play damage animation
	// - Spawn damage numbers UI
	// - Play hurt sound
	// - Check for death
	if (ResourceComponent && ResourceComponent->GetCurrentResource(EResourceType::Health) <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());
		// Handle death
	}
}

void ACombatCharacter::OnAttackExecuted(EAttackType AttackType, AActor* Target, float Damage)
{
	// Log attack for debugging
	FString AttackTypeName;
	switch (AttackType)
	{
	case EAttackType::MeleeAttack:
		AttackTypeName = "Melee";
		break;
	case EAttackType::RangedAttack:
		AttackTypeName = "Ranged";
		break;
	case EAttackType::MagicalAttack:
		AttackTypeName = "Magical";
		break;
	}

	UE_LOG(LogTemp, Log, TEXT("%s executed %s attack on %s for %.2f damage"), 
		*GetName(), 
		*AttackTypeName, 
		Target ? *Target->GetName() : TEXT("Unknown"), 
		Damage);

	// Here you could:
	// - Play attack animation
	// - Spawn projectile for ranged/magical attacks
	// - Play attack sound/VFX
}

void ACombatCharacter::OnResourceChanged(EResourceType ResourceType, float CurrentValue, float MaxValue)
{
	// Log resource change for debugging
	FString ResourceName;
	switch (ResourceType)
	{
	case EResourceType::Health:
		ResourceName = "Health";
		break;
	case EResourceType::Mana:
		ResourceName = "Mana";
		break;
	case EResourceType::Stamina:
		ResourceName = "Stamina";
		break;
	}

	UE_LOG(LogTemp, Log, TEXT("%s %s: %.2f / %.2f"), 
		*GetName(), 
		*ResourceName, 
		CurrentValue, 
		MaxValue);

	// Here you could:
	// - Update UI bars
	// - Trigger low resource warnings
	// - Enable/disable abilities based on resources
}
