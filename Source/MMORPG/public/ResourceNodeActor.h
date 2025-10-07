// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceTypes.h"
#include "ResourceNodeActor.generated.h"

/**
 * Actor representing a resource node that can be gathered from
 */
UCLASS()
class MMORPG_API AResourceNodeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceNodeActor();

protected:
	virtual void BeginPlay() override;

public:	
	/**
	 * The type of resource this node provides
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node")
	EResourceType ResourceType = EResourceType::Wood;

	/**
	 * The current quantity of resources available in this node
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node")
	int32 CurrentQuantity = 100;

	/**
	 * The maximum quantity this node can hold
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node")
	int32 MaxQuantity = 100;

	/**
	 * Whether this node regenerates resources over time
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node")
	bool bRegenerates = true;

	/**
	 * Time in seconds between regeneration ticks
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node", meta = (EditCondition = "bRegenerates"))
	float RegenerationInterval = 60.0f;

	/**
	 * Amount of resources regenerated per tick
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node", meta = (EditCondition = "bRegenerates"))
	int32 RegenerationAmount = 10;

	/**
	 * The skill type associated with gathering from this node
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node")
	EGatheringSkillType AssociatedSkill = EGatheringSkillType::Woodcutting;

	/**
	 * Experience points awarded for successfully gathering from this node
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Node")
	int32 ExperiencePerGather = 10;

	/**
	 * Attempt to gather resources from this node
	 * @param AmountToGather - The amount to attempt to gather
	 * @return The actual amount gathered (may be less than requested)
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource Node")
	int32 GatherResource(int32 AmountToGather = 1);

	/**
	 * Check if the node has resources available
	 * @return True if there are resources available to gather
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resource Node")
	bool HasResourcesAvailable() const;

	/**
	 * Get the remaining resources in this node
	 * @return The current quantity of resources
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resource Node")
	int32 GetRemainingResources() const { return CurrentQuantity; }

private:
	/**
	 * Handle for the regeneration timer
	 */
	FTimerHandle RegenerationTimerHandle;

	/**
	 * Regenerate resources
	 */
	void RegenerateResources();
};
