// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "ExampleWeapons.generated.h"

/**
 * Example: Iron Sword - Basic melee weapon
 */
UCLASS(Blueprintable)
class MMORPG_API AIronSword : public AWeaponItem
{
	GENERATED_BODY()

public:
	AIronSword();
};

/**
 * Example: Longbow - Basic ranged weapon
 */
UCLASS(Blueprintable)
class MMORPG_API ALongbow : public AWeaponItem
{
	GENERATED_BODY()

public:
	ALongbow();
};

/**
 * Example: Fire Staff - Basic magical weapon
 */
UCLASS(Blueprintable)
class MMORPG_API AFireStaff : public AWeaponItem
{
	GENERATED_BODY()

public:
	AFireStaff();
};

/**
 * Example: Legendary Greatsword - High-damage melee weapon
 */
UCLASS(Blueprintable)
class MMORPG_API AGreatsword : public AWeaponItem
{
	GENERATED_BODY()

public:
	AGreatsword();
};
