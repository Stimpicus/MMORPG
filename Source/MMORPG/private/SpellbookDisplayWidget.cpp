// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpellbookDisplayWidget.h"

void USpellbookDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize with placeholder spells
	InitializePlaceholderSpells();
}

void USpellbookDisplayWidget::InitializePlaceholderSpells()
{
	AllSpells.Empty();

	// Fire Spells
	FSpellData Fireball;
	Fireball.SpellName = TEXT("Fireball");
	Fireball.SpellSchool = TEXT("Fire");
	Fireball.ManaCost = 25;
	Fireball.SpellLevel = 1;
	Fireball.RequiredMagicLevel = 5;
	Fireball.Cooldown = 2.0f;
	Fireball.Description = TEXT("Hurls a ball of fire at the target, dealing fire damage");
	Fireball.bIsLearned = true;
	Fireball.SpellType = TEXT("Offensive");
	AllSpells.Add(Fireball);

	FSpellData Inferno;
	Inferno.SpellName = TEXT("Inferno");
	Inferno.SpellSchool = TEXT("Fire");
	Inferno.ManaCost = 50;
	Inferno.SpellLevel = 3;
	Inferno.RequiredMagicLevel = 15;
	Inferno.Cooldown = 10.0f;
	Inferno.Description = TEXT("Creates a massive explosion of flames");
	Inferno.bIsLearned = false;
	Inferno.SpellType = TEXT("Offensive");
	AllSpells.Add(Inferno);

	// Ice Spells
	FSpellData FrostBolt;
	FrostBolt.SpellName = TEXT("Frost Bolt");
	FrostBolt.SpellSchool = TEXT("Ice");
	FrostBolt.ManaCost = 20;
	FrostBolt.SpellLevel = 1;
	FrostBolt.RequiredMagicLevel = 3;
	FrostBolt.Cooldown = 1.5f;
	FrostBolt.Description = TEXT("Shoots a bolt of frost, slowing the target");
	FrostBolt.bIsLearned = true;
	FrostBolt.SpellType = TEXT("Offensive");
	AllSpells.Add(FrostBolt);

	FSpellData IceBarrier;
	IceBarrier.SpellName = TEXT("Ice Barrier");
	IceBarrier.SpellSchool = TEXT("Ice");
	IceBarrier.ManaCost = 30;
	IceBarrier.SpellLevel = 2;
	IceBarrier.RequiredMagicLevel = 10;
	IceBarrier.Cooldown = 30.0f;
	IceBarrier.Description = TEXT("Creates a protective barrier of ice");
	IceBarrier.bIsLearned = true;
	IceBarrier.SpellType = TEXT("Defensive");
	AllSpells.Add(IceBarrier);

	// Lightning Spells
	FSpellData LightningBolt;
	LightningBolt.SpellName = TEXT("Lightning Bolt");
	LightningBolt.SpellSchool = TEXT("Lightning");
	LightningBolt.ManaCost = 35;
	LightningBolt.SpellLevel = 2;
	LightningBolt.RequiredMagicLevel = 8;
	LightningBolt.Cooldown = 3.0f;
	LightningBolt.Description = TEXT("Strikes the target with a powerful bolt of lightning");
	LightningBolt.bIsLearned = true;
	LightningBolt.SpellType = TEXT("Offensive");
	AllSpells.Add(LightningBolt);

	FSpellData ChainLightning;
	ChainLightning.SpellName = TEXT("Chain Lightning");
	ChainLightning.SpellSchool = TEXT("Lightning");
	ChainLightning.ManaCost = 60;
	ChainLightning.SpellLevel = 4;
	ChainLightning.RequiredMagicLevel = 20;
	ChainLightning.Cooldown = 15.0f;
	ChainLightning.Description = TEXT("Lightning that jumps between multiple targets");
	ChainLightning.bIsLearned = false;
	ChainLightning.SpellType = TEXT("Offensive");
	AllSpells.Add(ChainLightning);

	// Healing Spells
	FSpellData Heal;
	Heal.SpellName = TEXT("Heal");
	Heal.SpellSchool = TEXT("Holy");
	Heal.ManaCost = 15;
	Heal.SpellLevel = 1;
	Heal.RequiredMagicLevel = 1;
	Heal.Cooldown = 1.0f;
	Heal.Description = TEXT("Restores health to the target");
	Heal.bIsLearned = true;
	Heal.SpellType = TEXT("Healing");
	AllSpells.Add(Heal);

	FSpellData GreaterHeal;
	GreaterHeal.SpellName = TEXT("Greater Heal");
	GreaterHeal.SpellSchool = TEXT("Holy");
	GreaterHeal.ManaCost = 40;
	GreaterHeal.SpellLevel = 3;
	GreaterHeal.RequiredMagicLevel = 12;
	GreaterHeal.Cooldown = 5.0f;
	GreaterHeal.Description = TEXT("Restores a large amount of health");
	GreaterHeal.bIsLearned = false;
	GreaterHeal.SpellType = TEXT("Healing");
	AllSpells.Add(GreaterHeal);

	// Utility Spells
	FSpellData Teleport;
	Teleport.SpellName = TEXT("Teleport");
	Teleport.SpellSchool = TEXT("Arcane");
	Teleport.ManaCost = 45;
	Teleport.SpellLevel = 2;
	Teleport.RequiredMagicLevel = 10;
	Teleport.Cooldown = 60.0f;
	Teleport.Description = TEXT("Instantly transports the caster to a nearby location");
	Teleport.bIsLearned = false;
	Teleport.SpellType = TEXT("Utility");
	AllSpells.Add(Teleport);

	FSpellData ManaShield;
	ManaShield.SpellName = TEXT("Mana Shield");
	ManaShield.SpellSchool = TEXT("Arcane");
	ManaShield.ManaCost = 20;
	ManaShield.SpellLevel = 1;
	ManaShield.RequiredMagicLevel = 5;
	ManaShield.Cooldown = 20.0f;
	ManaShield.Description = TEXT("Absorbs damage using mana");
	ManaShield.bIsLearned = true;
	ManaShield.SpellType = TEXT("Defensive");
	AllSpells.Add(ManaShield);

	// Initialize equipped spells with some learned spells
	EquippedSpells.Empty();
	EquippedSpells.Add(Fireball);
	EquippedSpells.Add(FrostBolt);
	EquippedSpells.Add(LightningBolt);
	EquippedSpells.Add(Heal);
}

void USpellbookDisplayWidget::RefreshSpellbook()
{
	// Placeholder: In the future, this will fetch data from spell manager
}

TArray<FSpellData> USpellbookDisplayWidget::GetLearnedSpells() const
{
	TArray<FSpellData> LearnedSpells;
	for (const FSpellData& Spell : AllSpells)
	{
		if (Spell.bIsLearned)
		{
			LearnedSpells.Add(Spell);
		}
	}
	return LearnedSpells;
}

TArray<FSpellData> USpellbookDisplayWidget::GetSpellsBySchool(const FString& SchoolName) const
{
	TArray<FSpellData> SchoolSpells;
	for (const FSpellData& Spell : AllSpells)
	{
		if (Spell.SpellSchool.Equals(SchoolName))
		{
			SchoolSpells.Add(Spell);
		}
	}
	return SchoolSpells;
}

FSpellData USpellbookDisplayWidget::GetSpellByName(const FString& SpellName) const
{
	for (const FSpellData& Spell : AllSpells)
	{
		if (Spell.SpellName.Equals(SpellName))
		{
			return Spell;
		}
	}
	return FSpellData();
}

bool USpellbookDisplayWidget::LearnSpell(const FString& SpellName)
{
	for (FSpellData& Spell : AllSpells)
	{
		if (Spell.SpellName.Equals(SpellName) && !Spell.bIsLearned)
		{
			Spell.bIsLearned = true;
			return true;
		}
	}
	return false;
}

bool USpellbookDisplayWidget::EquipSpell(const FSpellData& Spell)
{
	if (!Spell.bIsLearned)
	{
		return false;
	}

	if (EquippedSpells.Num() >= MaxEquippedSpells)
	{
		return false; // All slots full
	}

	EquippedSpells.Add(Spell);
	return true;
}

bool USpellbookDisplayWidget::UnequipSpell(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < EquippedSpells.Num())
	{
		EquippedSpells.RemoveAt(SlotIndex);
		return true;
	}
	return false;
}

int32 USpellbookDisplayWidget::GetLearnedSpellCount() const
{
	int32 Count = 0;
	for (const FSpellData& Spell : AllSpells)
	{
		if (Spell.bIsLearned)
		{
			++Count;
		}
	}
	return Count;
}

bool USpellbookDisplayWidget::CanLearnSpell(const FSpellData& Spell, int32 PlayerMagicLevel) const
{
	return PlayerMagicLevel >= Spell.RequiredMagicLevel && !Spell.bIsLearned;
}
