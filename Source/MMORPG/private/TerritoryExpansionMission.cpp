// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerritoryExpansionMission.h"

UTerritoryExpansionMission::UTerritoryExpansionMission()
{
	MissionType = EMissionType::TerritoryExpansion;
	bIsInstanced = true;
	MissionName = FName(TEXT("Territory Expansion"));
	MissionDescription = FText::FromString(TEXT("Escort the convoy through enemy territory to expand faction control."));
}

void UTerritoryExpansionMission::InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction)
{
	Super::InitializeMission(InTerritory, InFaction);
	CheckpointsReached = 0;
	ConvoyHealth = ConvoyMaxHealth;
}

void UTerritoryExpansionMission::ReachCheckpoint()
{
	if (MissionStatus == EMissionStatus::Active)
	{
		CheckpointsReached++;
		
		if (CheckpointsReached >= CheckpointsTotal)
		{
			CompleteMission();
		}
	}
}

void UTerritoryExpansionMission::DamageConvoy(float Damage)
{
	if (MissionStatus == EMissionStatus::Active)
	{
		ConvoyHealth = FMath::Max(0.0f, ConvoyHealth - Damage);
		
		if (ConvoyHealth <= 0.0f)
		{
			FailMission();
		}
	}
}
