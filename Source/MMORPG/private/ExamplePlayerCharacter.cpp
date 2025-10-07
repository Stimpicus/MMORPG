// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExamplePlayerCharacter.h"
#include "InventoryComponent.h"
#include "SkillProgressionComponent.h"
#include "ResourceNodeActor.h"
#include "ResourceGatheringSubsystem.h"

AExamplePlayerCharacter::AExamplePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the inventory component
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	// Optionally set a max stack size (0 = unlimited)
	Inventory->MaxStackSize = 0;

	// Create the skill progression component
	SkillProgression = CreateDefaultSubobject<USkillProgressionComponent>(TEXT("SkillProgression"));
	
	// Configure skill progression settings
	SkillProgression->BaseExperienceForLevel = 100;
	SkillProgression->ExperienceMultiplierPerLevel = 1.5f;
}

void AExamplePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize gathering skills
	SkillProgression->InitializeSkill(EGatheringSkillType::Mining);
	SkillProgression->InitializeSkill(EGatheringSkillType::Herbalism);
	SkillProgression->InitializeSkill(EGatheringSkillType::Woodcutting);
}

void AExamplePlayerCharacter::GatherFromNode(AResourceNodeActor* ResourceNode)
{
	if (!ResourceNode)
	{
		return;
	}

	// Check if we can gather from this node
	if (!UResourceGatheringSubsystem::CanGatherFrom(ResourceNode, Inventory))
	{
		// Node has no resources or inventory is full
		return;
	}

	// Perform the gathering operation
	FGatheringResult Result = UResourceGatheringSubsystem::PerformGathering(
		ResourceNode,
		Inventory,
		SkillProgression,
		1 // Gather 1 resource at a time
	);

	// Handle the result
	if (Result.bSuccess)
	{
		// Successfully gathered resources
		// You could add UI feedback, sound effects, particles, etc. here
		
		if (Result.bLeveledUp)
		{
			// Player leveled up a skill!
			// Show a level-up notification
			EGatheringSkillType Skill = ResourceNode->AssociatedSkill;
			int32 NewLevel = SkillProgression->GetSkillLevel(Skill);
			
			// Example: Log the level up (in a real game, show UI notification)
			UE_LOG(LogTemp, Log, TEXT("Skill leveled up to %d!"), NewLevel);
		}
	}
}

int32 AExamplePlayerCharacter::GetResourceCount(EResourceType ResourceType) const
{
	if (Inventory)
	{
		return Inventory->GetResourceQuantity(ResourceType);
	}
	return 0;
}

int32 AExamplePlayerCharacter::GetGatheringSkillLevel(EGatheringSkillType SkillType) const
{
	if (SkillProgression)
	{
		return SkillProgression->GetSkillLevel(SkillType);
	}
	return 1;
}
