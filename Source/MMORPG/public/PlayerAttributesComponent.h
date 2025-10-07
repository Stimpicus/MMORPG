// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttributesComponent.generated.h"

/**
 * Component that manages persistent player attributes such as XP, HP, Mana, Stamina, and Credits.
 * Designed to be modular and support networking and persistence.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UPlayerAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerAttributesComponent();

	// Initialize default values
	virtual void BeginPlay() override;

	// Getters for attributes
	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetXP() const { return CurrentXP; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHP() const { return CurrentHP; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMana() const { return CurrentMana; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxMana() const { return MaxMana; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	int32 GetCredits() const { return Credits; }

	// Setters and modifiers
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddXP(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ModifyHP(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ModifyMana(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ModifyStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddCredits(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool SpendCredits(int32 Amount);

	// Equipment modifier hooks
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyMaxHPModifier(float Modifier);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyMaxManaModifier(float Modifier);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyMaxStaminaModifier(float Modifier);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void RecalculateMaxValues();

protected:
	// Base persistent attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes|Persistent")
	float CurrentXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes|Persistent")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes|Persistent")
	float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes|Persistent")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes|Persistent")
	int32 Credits;

	// Base max values (before modifiers)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes|Base")
	float BaseMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes|Base")
	float BaseMaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes|Base")
	float BaseMaxStamina;

	// Current max values (after modifiers)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Current")
	float MaxHP;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Current")
	float MaxMana;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Current")
	float MaxStamina;

	// Equipment modifiers
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Modifiers")
	float EquipmentMaxHPModifier;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Modifiers")
	float EquipmentMaxManaModifier;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Modifiers")
	float EquipmentMaxStaminaModifier;

	// Networking support
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
