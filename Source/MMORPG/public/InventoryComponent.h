// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceTypes.h"
#include "InventoryComponent.generated.h"

/**
 * Component that manages a collection of resources for an actor
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	/**
	 * Map of resource types to their quantities
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<EResourceType, int32> ResourceInventory;

	/**
	 * Maximum stack size for a single resource type (0 = unlimited)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxStackSize = 0;

	/**
	 * Add a resource to the inventory
	 * @param ResourceType - The type of resource to add
	 * @param Quantity - The amount to add
	 * @return True if the resource was successfully added
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddResource(EResourceType ResourceType, int32 Quantity);

	/**
	 * Add a resource item to the inventory
	 * @param ResourceItem - The resource item to add
	 * @return True if the resource was successfully added
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddResourceItem(const FResourceItem& ResourceItem);

	/**
	 * Remove a resource from the inventory
	 * @param ResourceType - The type of resource to remove
	 * @param Quantity - The amount to remove
	 * @return The actual amount removed (may be less than requested)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 RemoveResource(EResourceType ResourceType, int32 Quantity);

	/**
	 * Get the quantity of a specific resource type
	 * @param ResourceType - The type of resource to query
	 * @return The quantity of the resource in inventory
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetResourceQuantity(EResourceType ResourceType) const;

	/**
	 * Check if the inventory has a minimum quantity of a resource
	 * @param ResourceType - The type of resource to check
	 * @param MinQuantity - The minimum quantity required
	 * @return True if the inventory has at least MinQuantity of the resource
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasResource(EResourceType ResourceType, int32 MinQuantity = 1) const;

	/**
	 * Clear all resources from the inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();

	/**
	 * Get all resources in the inventory
	 * @return Map of all resources and their quantities
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TMap<EResourceType, int32> GetAllResources() const { return ResourceInventory; }
};
