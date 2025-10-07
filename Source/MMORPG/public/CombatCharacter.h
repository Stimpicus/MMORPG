// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

class UResourceComponent;
class UCombatComponent;

/**
 * Example character class with combat capabilities
 * This demonstrates how to integrate the combat system with a player character
 */
UCLASS()
class MMORPG_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacter();

	virtual void BeginPlay() override;

	// Attack input handlers
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformRangedAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMagicalAttack();

	// Equip weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(const FWeaponData& Weapon);

	// Get current target (simple implementation)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCurrentTarget(AActor* Target) { CurrentTarget = Target; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UResourceComponent* ResourceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UCombatComponent* CombatComponent;

	UPROPERTY()
	AActor* CurrentTarget;

private:
	// Callback for when damage is taken
	UFUNCTION()
	void OnDamageTaken(AActor* Instigator, float Damage);

	// Callback for when attack is executed
	UFUNCTION()
	void OnAttackExecuted(EAttackType AttackType, AActor* Target, float Damage);

	// Callback for resource changes (for UI updates)
	UFUNCTION()
	void OnResourceChanged(EResourceType ResourceType, float CurrentValue, float MaxValue);
};
