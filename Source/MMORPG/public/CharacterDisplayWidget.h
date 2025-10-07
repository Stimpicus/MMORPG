// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDisplayWidget.generated.h"

/**
 * Character Display Panel Widget
 * Displays player character attributes, equipped items, and basic stats
 */
UCLASS()
class MMORPG_API UCharacterDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Character Attributes (Placeholder values)
	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	FString CharacterName = TEXT("Hero");

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 Level = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 Health = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 Mana = 50;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 MaxMana = 50;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 Experience = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Attributes")
	int32 ExperienceToNextLevel = 100;

	// Primary Stats
	UPROPERTY(BlueprintReadWrite, Category = "Character|Stats")
	int32 Strength = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Stats")
	int32 Dexterity = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Stats")
	int32 Intelligence = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Stats")
	int32 Constitution = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Stats")
	int32 Wisdom = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Stats")
	int32 Charisma = 10;

	// Equipped Items (Placeholder strings)
	UPROPERTY(BlueprintReadWrite, Category = "Character|Equipment")
	FString EquippedWeapon = TEXT("Iron Sword");

	UPROPERTY(BlueprintReadWrite, Category = "Character|Equipment")
	FString EquippedArmor = TEXT("Leather Armor");

	UPROPERTY(BlueprintReadWrite, Category = "Character|Equipment")
	FString EquippedHelmet = TEXT("None");

	UPROPERTY(BlueprintReadWrite, Category = "Character|Equipment")
	FString EquippedBoots = TEXT("Worn Boots");

	UPROPERTY(BlueprintReadWrite, Category = "Character|Equipment")
	FString EquippedGloves = TEXT("None");

	UPROPERTY(BlueprintReadWrite, Category = "Character|Equipment")
	FString EquippedAccessory = TEXT("None");

	// Blueprint-callable functions for future data integration
	UFUNCTION(BlueprintCallable, Category = "Character")
	void RefreshCharacterData();

	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetManaPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetExperiencePercentage() const;

protected:
	virtual void NativeConstruct() override;
};
