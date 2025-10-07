// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceGatheringSubsystem.h"
#include "ResourceNodeActor.h"
#include "InventoryComponent.h"
#include "SkillProgressionComponent.h"

FGatheringResult UResourceGatheringSubsystem::PerformGathering(
	AResourceNodeActor* ResourceNode,
	UInventoryComponent* Inventory,
	USkillProgressionComponent* SkillProgression,
	int32 AmountToGather)
{
	FGatheringResult Result;

	// Validate inputs
	if (!ResourceNode || !Inventory || AmountToGather <= 0)
	{
		return Result;
	}

	// Check if the node has resources
	if (!ResourceNode->HasResourcesAvailable())
	{
		return Result;
	}

	// Attempt to gather from the node
	int32 AmountGathered = ResourceNode->GatherResource(AmountToGather);
	
	if (AmountGathered <= 0)
	{
		return Result;
	}

	// Add the gathered resources to inventory
	bool bAddedToInventory = Inventory->AddResource(ResourceNode->ResourceType, AmountGathered);
	
	if (!bAddedToInventory)
	{
		// Failed to add to inventory, return resources to node
		// This is a simple approach - a more sophisticated system might handle this differently
		return Result;
	}

	// Success!
	Result.bSuccess = true;
	Result.AmountGathered = AmountGathered;

	// Add experience if skill progression is available
	if (SkillProgression && ResourceNode->AssociatedSkill != EGatheringSkillType::None)
	{
		int32 ExperienceToGain = ResourceNode->ExperiencePerGather * AmountGathered;
		Result.ExperienceGained = ExperienceToGain;
		Result.bLeveledUp = SkillProgression->AddSkillExperience(
			ResourceNode->AssociatedSkill,
			ExperienceToGain
		);
	}

	return Result;
}

bool UResourceGatheringSubsystem::CanGatherFrom(
	AResourceNodeActor* ResourceNode,
	UInventoryComponent* Inventory)
{
	if (!ResourceNode)
	{
		return false;
	}

	if (!ResourceNode->HasResourcesAvailable())
	{
		return false;
	}

	// If inventory is provided, we could check capacity here
	// For now, we just check if the node has resources
	
	return true;
}
