// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerritoryActor.generated.h"

class UFactionDataAsset;

/**
 * Actor representing a territory in the game world
 * Manages faction ownership and player employment rules
 */
UCLASS(BlueprintType)
class MMORPG_API ATerritoryActor : public AActor
{
	GENERATED_BODY()

public:
	ATerritoryActor();

	/** Get the current owning faction */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	UFactionDataAsset* GetOwningFaction() const { return OwningFaction; }

	/** Set the owning faction of this territory */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	void SetOwningFaction(UFactionDataAsset* NewFaction);

	/** Check if a player with given faction can be employed in this territory */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	bool CanPlayerBeEmployed(UFactionDataAsset* PlayerFaction) const;

	/** Territory name/identifier */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Territory")
	FName TerritoryName;

	/** Territory description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Territory", meta = (MultiLine = true))
	FText TerritoryDescription;

protected:
	/** Current faction that owns this territory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory")
	UFactionDataAsset* OwningFaction;

	/** Whether neutral faction (Syndicate) players can always work here */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Territory")
	bool bAllowNeutralFaction = true;
};
