// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MissionBase.h"
#include "TerritoryClearingMission.generated.h"

/**
 * Territory Clearing Mission - Instanced PvE
 * Players clear enemies from a territory instance
 */
UCLASS(Blueprintable, BlueprintType)
class MMORPG_API UTerritoryClearingMission : public UMissionBase
{
	GENERATED_BODY()

public:
	UTerritoryClearingMission();

	virtual void InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction) override;

	/** Number of enemies to clear */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Territory Clearing")
	int32 EnemiesToClear = 10;

	/** Current enemies cleared */
	UPROPERTY(BlueprintReadOnly, Category = "Mission|Territory Clearing")
	int32 EnemiesCleared = 0;

	/** Add to cleared enemy count */
	UFUNCTION(BlueprintCallable, Category = "Mission|Territory Clearing")
	void AddEnemyCleared();
};
