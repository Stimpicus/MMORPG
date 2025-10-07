// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellbookDisplayWidget.generated.h"

/**
 * Spell data structure for displaying available spells
 */
USTRUCT(BlueprintType)
struct FSpellData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	FString SpellName = TEXT("Unknown Spell");

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	FString SpellSchool = TEXT("Arcane");

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	int32 ManaCost = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	int32 SpellLevel = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	int32 RequiredMagicLevel = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	float Cooldown = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	FString Description = TEXT("A magical spell");

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	bool bIsLearned = false;

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	FString SpellType = TEXT("Offensive");
};

/**
 * Spellbook Display Panel Widget
 * Lists available spells and learned spells
 */
UCLASS()
class MMORPG_API USpellbookDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// All available spells
	UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
	TArray<FSpellData> AllSpells;

	// Currently equipped spells (quick slots)
	UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
	TArray<FSpellData> EquippedSpells;

	UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
	int32 MaxEquippedSpells = 8;

	// Blueprint-callable functions
	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	void RefreshSpellbook();

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	TArray<FSpellData> GetLearnedSpells() const;

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	TArray<FSpellData> GetSpellsBySchool(const FString& SchoolName) const;

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	FSpellData GetSpellByName(const FString& SpellName) const;

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	bool LearnSpell(const FString& SpellName);

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	bool EquipSpell(const FSpellData& Spell);

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	bool UnequipSpell(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	int32 GetLearnedSpellCount() const;

	UFUNCTION(BlueprintCallable, Category = "Spellbook")
	bool CanLearnSpell(const FSpellData& Spell, int32 PlayerMagicLevel) const;

protected:
	virtual void NativeConstruct() override;

private:
	void InitializePlaceholderSpells();
};
