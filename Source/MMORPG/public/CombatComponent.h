// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatTypes.h"
#include "CombatComponent.generated.h"

class UResourceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttackExecuted, EAttackType, AttackType, AActor*, Target, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTaken, AActor*, Instigator, float, Damage);

/**
 * Component that handles combat abilities and attacks
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Execute an attack ability
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool ExecuteAttack(const FAttackAbilityData& AbilityData, AActor* Target);

	// Execute a melee attack
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool ExecuteMeleeAttack(AActor* Target);

	// Execute a ranged attack
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool ExecuteRangedAttack(AActor* Target);

	// Execute a magical attack
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool ExecuteMagicalAttack(AActor* Target);

	// Apply damage to this actor
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float Damage, AActor* DamageInstigator);

	// Check if an ability is on cooldown
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAbilityOnCooldown(EAttackType AttackType) const;

	// Get remaining cooldown time
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetRemainingCooldown(EAttackType AttackType) const;

	// Set equipped weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetEquippedWeapon(const FWeaponData& Weapon);

	// Get equipped weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	FWeaponData GetEquippedWeapon() const { return EquippedWeapon; }

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackExecuted OnAttackExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnDamageTaken OnDamageTaken;

protected:
	// Reference to resource component
	UPROPERTY()
	UResourceComponent* ResourceComponent;

	// Currently equipped weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FWeaponData EquippedWeapon;

	// Attack abilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FAttackAbilityData MeleeAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FAttackAbilityData RangedAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FAttackAbilityData MagicalAbility;

	// Cooldown tracking
	UPROPERTY()
	TMap<EAttackType, float> AbilityCooldowns;

	// Helper functions
	bool CanExecuteAttack(const FAttackAbilityData& AbilityData, AActor* Target);
	void ApplyCooldown(EAttackType AttackType, float Cooldown);
	void UpdateCooldowns(float DeltaTime);
	bool IsTargetInRange(AActor* Target, float Range) const;
};
