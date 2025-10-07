// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MissionBase.h"
#include "ResourceCollectionMission.generated.h"

/**
 * Resource Collection Mission - Open World
 * Players collect resources in the open world
 */
UCLASS(Blueprintable, BlueprintType)
class MMORPG_API UResourceCollectionMission : public UMissionBase
{
	GENERATED_BODY()

public:
	UResourceCollectionMission();

	virtual void InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction) override;

	/** Type of resource to collect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Resource Collection")
	FName ResourceType;

	/** Number of resources required */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Resource Collection")
	int32 ResourcesRequired = 5;

	/** Current resources collected */
	UPROPERTY(BlueprintReadOnly, Category = "Mission|Resource Collection")
	int32 ResourcesCollected = 0;

	/** Add to collected resource count */
	UFUNCTION(BlueprintCallable, Category = "Mission|Resource Collection")
	void AddResourceCollected(int32 Amount = 1);
};
