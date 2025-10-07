// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerritoryActor.h"
#include "FactionDataAsset.h"

ATerritoryActor::ATerritoryActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATerritoryActor::SetOwningFaction(UFactionDataAsset* NewFaction)
{
	OwningFaction = NewFaction;
}

bool ATerritoryActor::CanPlayerBeEmployed(UFactionDataAsset* PlayerFaction) const
{
	if (!PlayerFaction || !OwningFaction)
	{
		return false;
	}

	// Syndicate (neutral faction) can work in any territory
	if (bAllowNeutralFaction && PlayerFaction->bIsNeutralFaction)
	{
		return true;
	}

	// Players can only work in territories owned by their faction
	return PlayerFaction == OwningFaction;
}
