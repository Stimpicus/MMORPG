// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMORPGGameMode.h"
#include "Characters/MMORPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMMORPGGameMode::AMMORPGGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AMMORPGCharacter::StaticClass();
}
