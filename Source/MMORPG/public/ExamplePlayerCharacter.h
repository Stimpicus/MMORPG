// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ResourceTypes.h"
#include "ExamplePlayerCharacter.generated.h"

class UInventoryComponent;
class USkillProgressionComponent;
class AResourceNodeActor;

/**
 * Example player character demonstrating resource gathering system integration
 * 
 * This is a minimal example showing how to use the resource gathering components.
 * In a real implementation, you would likely add this to your existing player character.
 */
UCLASS()
class MMORPG_API AExamplePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AExamplePlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	/**
	 * Player's inventory component for storing gathered resources
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Components")
	UInventoryComponent* Inventory;

	/**
	 * Player's skill progression component for tracking gathering skills
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Components")
	USkillProgressionComponent* SkillProgression;

	/**
	 * Interact with a resource node to gather resources
	 * @param ResourceNode - The node to gather from
	 */
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void GatherFromNode(AResourceNodeActor* ResourceNode);

	/**
	 * Get the total count of a specific resource type in inventory
	 * @param ResourceType - The type of resource to query
	 * @return The quantity of the resource
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player Actions")
	int32 GetResourceCount(EResourceType ResourceType) const;

	/**
	 * Get the current level of a gathering skill
	 * @param SkillType - The skill to query
	 * @return The current level of the skill
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player Actions")
	int32 GetGatheringSkillLevel(EGatheringSkillType SkillType) const;
};
