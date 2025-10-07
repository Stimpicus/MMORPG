// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatComponent.h"
#include "ResourceComponent.h"
#include "GameFramework/Actor.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize default melee ability
	MeleeAbility.AbilityName = "Melee Attack";
	MeleeAbility.AttackType = EAttackType::MeleeAttack;
	MeleeAbility.Damage = 15.0f;
	MeleeAbility.Range = 150.0f;
	MeleeAbility.Cooldown = 1.0f;
	FResourceCost MeleeStaminaCost;
	MeleeStaminaCost.ResourceType = EResourceType::Stamina;
	MeleeStaminaCost.Amount = 10.0f;
	MeleeAbility.ResourceCosts.Add(MeleeStaminaCost);

	// Initialize default ranged ability
	RangedAbility.AbilityName = "Ranged Attack";
	RangedAbility.AttackType = EAttackType::RangedAttack;
	RangedAbility.Damage = 20.0f;
	RangedAbility.Range = 500.0f;
	RangedAbility.Cooldown = 1.5f;
	FResourceCost RangedStaminaCost;
	RangedStaminaCost.ResourceType = EResourceType::Stamina;
	RangedStaminaCost.Amount = 15.0f;
	RangedAbility.ResourceCosts.Add(RangedStaminaCost);

	// Initialize default magical ability
	MagicalAbility.AbilityName = "Magical Attack";
	MagicalAbility.AttackType = EAttackType::MagicalAttack;
	MagicalAbility.Damage = 30.0f;
	MagicalAbility.Range = 400.0f;
	MagicalAbility.Cooldown = 2.0f;
	FResourceCost MagicalManaCost;
	MagicalManaCost.ResourceType = EResourceType::Mana;
	MagicalManaCost.Amount = 25.0f;
	MagicalAbility.ResourceCosts.Add(MagicalManaCost);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to resource component
	AActor* Owner = GetOwner();
	if (Owner)
	{
		ResourceComponent = Owner->FindComponentByClass<UResourceComponent>();
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCooldowns(DeltaTime);
}

bool UCombatComponent::ExecuteAttack(const FAttackAbilityData& AbilityData, AActor* Target)
{
	if (!CanExecuteAttack(AbilityData, Target))
	{
		return false;
	}

	// Consume resources
	if (ResourceComponent && !ResourceComponent->ConsumeResources(AbilityData.ResourceCosts))
	{
		return false;
	}

	// Apply cooldown
	ApplyCooldown(AbilityData.AttackType, AbilityData.Cooldown);

	// Calculate final damage (could be modified by weapon, stats, etc.)
	float FinalDamage = AbilityData.Damage;
	if (EquippedWeapon.WeaponType != EWeaponType::None)
	{
		FinalDamage += EquippedWeapon.BaseDamage;
	}

	// Apply damage to target
	if (Target)
	{
		UCombatComponent* TargetCombat = Target->FindComponentByClass<UCombatComponent>();
		if (TargetCombat)
		{
			TargetCombat->TakeDamage(FinalDamage, GetOwner());
		}
	}

	// Broadcast event
	OnAttackExecuted.Broadcast(AbilityData.AttackType, Target, FinalDamage);

	return true;
}

bool UCombatComponent::ExecuteMeleeAttack(AActor* Target)
{
	return ExecuteAttack(MeleeAbility, Target);
}

bool UCombatComponent::ExecuteRangedAttack(AActor* Target)
{
	return ExecuteAttack(RangedAbility, Target);
}

bool UCombatComponent::ExecuteMagicalAttack(AActor* Target)
{
	return ExecuteAttack(MagicalAbility, Target);
}

void UCombatComponent::TakeDamage(float Damage, AActor* DamageInstigator)
{
	if (ResourceComponent)
	{
		ResourceComponent->ConsumeResource(EResourceType::Health, Damage);
	}

	// Broadcast damage taken event
	OnDamageTaken.Broadcast(DamageInstigator, Damage);
}

bool UCombatComponent::IsAbilityOnCooldown(EAttackType AttackType) const
{
	const float* CooldownTime = AbilityCooldowns.Find(AttackType);
	return CooldownTime && *CooldownTime > 0.0f;
}

float UCombatComponent::GetRemainingCooldown(EAttackType AttackType) const
{
	const float* CooldownTime = AbilityCooldowns.Find(AttackType);
	return CooldownTime ? FMath::Max(0.0f, *CooldownTime) : 0.0f;
}

void UCombatComponent::SetEquippedWeapon(const FWeaponData& Weapon)
{
	EquippedWeapon = Weapon;

	// Update abilities based on weapon type
	switch (Weapon.WeaponType)
	{
	case EWeaponType::Melee:
		MeleeAbility.Range = Weapon.AttackRange;
		break;
	case EWeaponType::Ranged:
		RangedAbility.Range = Weapon.AttackRange;
		break;
	case EWeaponType::Magical:
		MagicalAbility.Range = Weapon.AttackRange;
		break;
	default:
		break;
	}
}

bool UCombatComponent::CanExecuteAttack(const FAttackAbilityData& AbilityData, AActor* Target)
{
	// Check if on cooldown
	if (IsAbilityOnCooldown(AbilityData.AttackType))
	{
		return false;
	}

	// Check if target is valid and in range
	if (!Target || !IsTargetInRange(Target, AbilityData.Range))
	{
		return false;
	}

	// Check if we have enough resources
	if (ResourceComponent && !ResourceComponent->HasEnoughResources(AbilityData.ResourceCosts))
	{
		return false;
	}

	return true;
}

void UCombatComponent::ApplyCooldown(EAttackType AttackType, float Cooldown)
{
	AbilityCooldowns.Add(AttackType, Cooldown);
}

void UCombatComponent::UpdateCooldowns(float DeltaTime)
{
	TArray<EAttackType> KeysToUpdate;
	AbilityCooldowns.GetKeys(KeysToUpdate);

	for (EAttackType AttackType : KeysToUpdate)
	{
		float& CooldownTime = AbilityCooldowns[AttackType];
		CooldownTime = FMath::Max(0.0f, CooldownTime - DeltaTime);
	}
}

bool UCombatComponent::IsTargetInRange(AActor* Target, float Range) const
{
	if (!Target || !GetOwner())
	{
		return false;
	}

	float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	return Distance <= Range;
}
