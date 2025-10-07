// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionManager.h"

UMissionBase* UMissionManager::CreateMission(TSubclassOf<UMissionBase> MissionClass, ATerritoryActor* Territory, UFactionDataAsset* Faction)
{
	if (!MissionClass)
	{
		return nullptr;
	}

	UMissionBase* NewMission = NewObject<UMissionBase>(this, MissionClass);
	if (NewMission)
	{
		NewMission->InitializeMission(Territory, Faction);
		AllMissions.Add(NewMission);
	}

	return NewMission;
}

TArray<UMissionBase*> UMissionManager::GetActiveMissions() const
{
	TArray<UMissionBase*> ActiveMissions;
	for (UMissionBase* Mission : AllMissions)
	{
		if (Mission && Mission->GetMissionStatus() == EMissionStatus::Active)
		{
			ActiveMissions.Add(Mission);
		}
	}
	return ActiveMissions;
}

TArray<UMissionBase*> UMissionManager::GetAvailableMissions() const
{
	TArray<UMissionBase*> AvailableMissions;
	for (UMissionBase* Mission : AllMissions)
	{
		if (Mission && Mission->GetMissionStatus() == EMissionStatus::Available)
		{
			AvailableMissions.Add(Mission);
		}
	}
	return AvailableMissions;
}

void UMissionManager::AwardMissionReward(UMissionBase* Mission, int32 PlayerCredits)
{
	// Stub for mission reward logic
	// In the future, this will connect to player persistence system
	if (Mission && Mission->GetMissionStatus() == EMissionStatus::Completed)
	{
		int32 Reward = Mission->GetMissionReward();
		// TODO: Add reward to player's credits through persistence system
		// For now, this is just a placeholder
	}
}
