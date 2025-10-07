// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionBase.h"
#include "FactionDataAsset.h"
#include "TerritoryActor.h"

UMissionBase::UMissionBase()
{
	MissionStatus = EMissionStatus::Available;
	BaseReward = 1000;
}

void UMissionBase::InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction)
{
	Territory = InTerritory;
	OfferingFaction = InFaction;
	MissionStatus = EMissionStatus::Available;
}

void UMissionBase::StartMission()
{
	if (MissionStatus == EMissionStatus::Available)
	{
		MissionStatus = EMissionStatus::Active;
	}
}

void UMissionBase::CompleteMission()
{
	if (MissionStatus == EMissionStatus::Active)
	{
		MissionStatus = EMissionStatus::Completed;
	}
}

void UMissionBase::FailMission()
{
	if (MissionStatus == EMissionStatus::Active)
	{
		MissionStatus = EMissionStatus::Failed;
	}
}

int32 UMissionBase::GetMissionReward() const
{
	// Fixed reward of 1,000 credits per mission
	// This is a stub for future implementation with variable rewards
	return BaseReward;
}
