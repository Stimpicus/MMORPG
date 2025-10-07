// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.generated.h"

/**
 * Weapon types available in the game
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None        UMETA(DisplayName = "None"),
	Melee       UMETA(DisplayName = "Melee"),
	Ranged      UMETA(DisplayName = "Ranged"),
	Magical     UMETA(DisplayName = "Magical")
};

/**
 * Attack ability types
 */
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	MeleeAttack     UMETA(DisplayName = "Melee Attack"),
	RangedAttack    UMETA(DisplayName = "Ranged Attack"),
	MagicalAttack   UMETA(DisplayName = "Magical Attack")
};

/**
 * Resource types for combat abilities
 */
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Health      UMETA(DisplayName = "Health"),
	Mana        UMETA(DisplayName = "Mana"),
	Stamina     UMETA(DisplayName = "Stamina")
};

/**
 * Weapon data structure
 */
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackSpeed;

	FWeaponData()
		: WeaponName("Default Weapon")
		, WeaponType(EWeaponType::None)
		, BaseDamage(10.0f)
		, AttackRange(100.0f)
		, AttackSpeed(1.0f)
	{}
};

/**
 * Resource cost for abilities
 */
USTRUCT(BlueprintType)
struct FResourceCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	EResourceType ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	float Amount;

	FResourceCost()
		: ResourceType(EResourceType::Stamina)
		, Amount(10.0f)
	{}
};

/**
 * Attack ability data
 */
USTRUCT(BlueprintType)
struct FAttackAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<FResourceCost> ResourceCosts;

	FAttackAbilityData()
		: AbilityName("Basic Attack")
		, AttackType(EAttackType::MeleeAttack)
		, Damage(10.0f)
		, Range(100.0f)
		, Cooldown(1.0f)
	{}
};
