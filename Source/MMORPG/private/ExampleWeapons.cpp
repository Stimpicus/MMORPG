// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleWeapons.h"

AIronSword::AIronSword()
{
	WeaponData.WeaponName = "Iron Sword";
	WeaponData.WeaponType = EWeaponType::Melee;
	WeaponData.BaseDamage = 15.0f;
	WeaponData.AttackRange = 150.0f;
	WeaponData.AttackSpeed = 1.2f;
}

ALongbow::ALongbow()
{
	WeaponData.WeaponName = "Longbow";
	WeaponData.WeaponType = EWeaponType::Ranged;
	WeaponData.BaseDamage = 18.0f;
	WeaponData.AttackRange = 600.0f;
	WeaponData.AttackSpeed = 0.8f;
}

AFireStaff::AFireStaff()
{
	WeaponData.WeaponName = "Fire Staff";
	WeaponData.WeaponType = EWeaponType::Magical;
	WeaponData.BaseDamage = 25.0f;
	WeaponData.AttackRange = 450.0f;
	WeaponData.AttackSpeed = 0.7f;
}

AGreatsword::AGreatsword()
{
	WeaponData.WeaponName = "Legendary Greatsword";
	WeaponData.WeaponType = EWeaponType::Melee;
	WeaponData.BaseDamage = 35.0f;
	WeaponData.AttackRange = 180.0f;
	WeaponData.AttackSpeed = 0.6f;
}
