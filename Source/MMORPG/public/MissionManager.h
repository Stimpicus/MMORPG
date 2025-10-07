// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionManager.generated.h"

class UMissionBase;
class UFactionDataAsset;
class ATerritoryActor;

/**
 * Mission Manager Subsystem
 * Manages mission creation, tracking, and rewards
 * Designed to be modular for future persistence and networking
 */
UCLASS()
class MMORPG_API UMissionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Create a new mission of the specified type */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	UMissionBase* CreateMission(TSubclassOf<UMissionBase> MissionClass, ATerritoryActor* Territory, UFactionDataAsset* Faction);

	/** Get all active missions */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	TArray<UMissionBase*> GetActiveMissions() const;

	/** Get all available missions */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	TArray<UMissionBase*> GetAvailableMissions() const;

	/** Award mission reward to player (stub for now) */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void AwardMissionReward(UMissionBase* Mission, int32 PlayerCredits);

protected:
	/** All missions in the system */
	UPROPERTY()
	TArray<UMissionBase*> AllMissions;
};
