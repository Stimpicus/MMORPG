// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceCollectionMission.h"

UResourceCollectionMission::UResourceCollectionMission()
{
	MissionType = EMissionType::ResourceCollection;
	bIsInstanced = false; // Open world
	MissionName = FName(TEXT("Resource Collection"));
	MissionDescription = FText::FromString(TEXT("Collect resources in the open world."));
	ResourceType = FName(TEXT("GenericResource"));
}

void UResourceCollectionMission::InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction)
{
	Super::InitializeMission(InTerritory, InFaction);
	ResourcesCollected = 0;
}

void UResourceCollectionMission::AddResourceCollected(int32 Amount)
{
	if (MissionStatus == EMissionStatus::Active)
	{
		ResourcesCollected += Amount;
		
		if (ResourcesCollected >= ResourcesRequired)
		{
			CompleteMission();
		}
	}
}
