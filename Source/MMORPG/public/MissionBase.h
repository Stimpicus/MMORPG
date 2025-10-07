// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MissionBase.generated.h"

class UFactionDataAsset;
class ATerritoryActor;

/**
 * Enum representing mission types
 */
UENUM(BlueprintType)
enum class EMissionType : uint8
{
	TerritoryClearing UMETA(DisplayName = "Territory Clearing (Instanced PvE)"),
	ResourceCollection UMETA(DisplayName = "Resource Collection (Open World)"),
	TerritoryExpansion UMETA(DisplayName = "Territory Expansion (Instanced Combat Escort)")
};

/**
 * Enum representing mission status
 */
UENUM(BlueprintType)
enum class EMissionStatus : uint8
{
	Available,
	Active,
	Completed,
	Failed
};

/**
 * Base class for all mission types in the game
 * Designed to be modular for future persistence and networking
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class MMORPG_API UMissionBase : public UObject
{
	GENERATED_BODY()

public:
	UMissionBase();

	/** Initialize the mission */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	virtual void InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction);

	/** Start the mission */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	virtual void StartMission();

	/** Complete the mission successfully */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	virtual void CompleteMission();

	/** Fail the mission */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	virtual void FailMission();

	/** Calculate and return mission reward (fixed at 1,000 credits for now) */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	virtual int32 GetMissionReward() const;

	/** Get the mission type */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	EMissionType GetMissionType() const { return MissionType; }

	/** Get the current status */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	EMissionStatus GetMissionStatus() const { return MissionStatus; }

	/** Mission name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	FName MissionName;

	/** Mission description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission", meta = (MultiLine = true))
	FText MissionDescription;

	/** Mission type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	EMissionType MissionType;

	/** Whether this mission is instanced */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	bool bIsInstanced = false;

protected:
	/** Fixed base reward in credits */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	int32 BaseReward = 1000;

	/** Current mission status */
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	EMissionStatus MissionStatus;

	/** Territory where this mission takes place */
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	ATerritoryActor* Territory;

	/** Faction offering this mission */
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	UFactionDataAsset* OfferingFaction;
};
