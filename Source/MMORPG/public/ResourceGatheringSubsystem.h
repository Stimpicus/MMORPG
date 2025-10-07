// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceTypes.h"
#include "ResourceGatheringSubsystem.generated.h"

class AResourceNodeActor;
class UInventoryComponent;
class USkillProgressionComponent;

/**
 * Result of a gathering attempt
 */
USTRUCT(BlueprintType)
struct FGatheringResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Gathering")
	bool bSuccess = false;

	UPROPERTY(BlueprintReadOnly, Category = "Gathering")
	int32 AmountGathered = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Gathering")
	int32 ExperienceGained = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Gathering")
	bool bLeveledUp = false;

	FGatheringResult()
		: bSuccess(false), AmountGathered(0), ExperienceGained(0), bLeveledUp(false)
	{
	}
};

/**
 * Subsystem for managing resource gathering operations
 * This provides a centralized way to handle gathering logic
 */
UCLASS()
class MMORPG_API UResourceGatheringSubsystem : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Perform a gathering operation on a resource node
	 * @param ResourceNode - The node to gather from
	 * @param Inventory - The inventory to add resources to
	 * @param SkillProgression - The skill progression component (optional)
	 * @param AmountToGather - The amount to attempt to gather
	 * @return The result of the gathering operation
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource Gathering")
	static FGatheringResult PerformGathering(
		AResourceNodeActor* ResourceNode,
		UInventoryComponent* Inventory,
		USkillProgressionComponent* SkillProgression = nullptr,
		int32 AmountToGather = 1
	);

	/**
	 * Check if gathering is possible from a node
	 * @param ResourceNode - The node to check
	 * @param Inventory - The inventory to check capacity (optional)
	 * @return True if gathering is possible
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resource Gathering")
	static bool CanGatherFrom(
		AResourceNodeActor* ResourceNode,
		UInventoryComponent* Inventory = nullptr
	);
};
