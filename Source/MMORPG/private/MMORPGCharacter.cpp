// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMORPGCharacter.h"
#include "PlayerAttributesComponent.h"
#include "PlayerSkillsComponent.h"
#include "EquipmentManagerComponent.h"

AMMORPGCharacter::AMMORPGCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and attach components
	AttributesComponent = CreateDefaultSubobject<UPlayerAttributesComponent>(TEXT("AttributesComponent"));
	SkillsComponent = CreateDefaultSubobject<UPlayerSkillsComponent>(TEXT("SkillsComponent"));
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
}

void AMMORPGCharacter::BeginPlay()
{
	Super::BeginPlay();
}
