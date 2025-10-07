// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.generated.h"

/**
 * Enumeration of resource types that can be gathered
 */
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	Iron UMETA(DisplayName = "Iron"),
	Herb UMETA(DisplayName = "Herb"),
	Crystal UMETA(DisplayName = "Crystal")
};

/**
 * Structure representing a resource item with type and quantity
 */
USTRUCT(BlueprintType)
struct FResourceItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	EResourceType ResourceType = EResourceType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Quantity = 0;

	FResourceItem()
		: ResourceType(EResourceType::None), Quantity(0)
	{
	}

	FResourceItem(EResourceType Type, int32 Amount)
		: ResourceType(Type), Quantity(Amount)
	{
	}
};

/**
 * Enumeration of gathering skill types
 */
UENUM(BlueprintType)
enum class EGatheringSkillType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Mining UMETA(DisplayName = "Mining"),
	Herbalism UMETA(DisplayName = "Herbalism"),
	Woodcutting UMETA(DisplayName = "Woodcutting")
};
