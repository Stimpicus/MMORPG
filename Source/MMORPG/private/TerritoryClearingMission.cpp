// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerritoryClearingMission.h"

UTerritoryClearingMission::UTerritoryClearingMission()
{
	MissionType = EMissionType::TerritoryClearing;
	bIsInstanced = true;
	MissionName = FName(TEXT("Territory Clearing"));
	MissionDescription = FText::FromString(TEXT("Clear enemies from the territory instance."));
}

void UTerritoryClearingMission::InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction)
{
	Super::InitializeMission(InTerritory, InFaction);
	EnemiesCleared = 0;
}

void UTerritoryClearingMission::AddEnemyCleared()
{
	if (MissionStatus == EMissionStatus::Active)
	{
		EnemiesCleared++;
		
		if (EnemiesCleared >= EnemiesToClear)
		{
			CompleteMission();
		}
	}
}
