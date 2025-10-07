// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeaponItem.h"

AWeaponItem::AWeaponItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create weapon mesh component
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// Default weapon data
	WeaponData.WeaponName = "Default Weapon";
	WeaponData.WeaponType = EWeaponType::Melee;
	WeaponData.BaseDamage = 10.0f;
	WeaponData.AttackRange = 150.0f;
	WeaponData.AttackSpeed = 1.0f;
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponItem::SetWeaponData(const FWeaponData& NewWeaponData)
{
	WeaponData = NewWeaponData;
}
