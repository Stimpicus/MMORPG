// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FactionDataAsset.generated.h"

/**
 * Enum representing faction relationships
 */
UENUM(BlueprintType)
enum class EFactionRelationship : uint8
{
	Neutral,
	Allied,
	Enemy
};

/**
 * Data asset that defines a faction in the game world
 */
UCLASS(BlueprintType)
class MMORPG_API UFactionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Name of the faction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
	FName FactionName;

	/** Description of the faction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction", meta = (MultiLine = true))
	FText FactionDescription;

	/** Whether this is the neutral faction (Syndicate) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
	bool bIsNeutralFaction = false;

	/** Color representing this faction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
	FLinearColor FactionColor;

	/** Base credit reward multiplier for this faction's missions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
	float RewardMultiplier = 1.0f;
};
