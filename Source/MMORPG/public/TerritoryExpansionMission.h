// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MissionBase.h"
#include "TerritoryExpansionMission.generated.h"

/**
 * Territory Expansion Mission - Instanced Combat Escort
 * Players escort a convoy through enemy territory to expand faction control
 */
UCLASS(Blueprintable, BlueprintType)
class MMORPG_API UTerritoryExpansionMission : public UMissionBase
{
	GENERATED_BODY()

public:
	UTerritoryExpansionMission();

	virtual void InitializeMission(ATerritoryActor* InTerritory, UFactionDataAsset* InFaction) override;

	/** Number of checkpoints to reach */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Territory Expansion")
	int32 CheckpointsTotal = 3;

	/** Current checkpoints reached */
	UPROPERTY(BlueprintReadOnly, Category = "Mission|Territory Expansion")
	int32 CheckpointsReached = 0;

	/** Convoy health (if it reaches 0, mission fails) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Territory Expansion")
	float ConvoyHealth = 100.0f;

	/** Maximum convoy health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Territory Expansion")
	float ConvoyMaxHealth = 100.0f;

	/** Reach a checkpoint */
	UFUNCTION(BlueprintCallable, Category = "Mission|Territory Expansion")
	void ReachCheckpoint();

	/** Damage the convoy */
	UFUNCTION(BlueprintCallable, Category = "Mission|Territory Expansion")
	void DamageConvoy(float Damage);
};
