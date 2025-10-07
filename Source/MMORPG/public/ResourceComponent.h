// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatTypes.h"
#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceChanged, EResourceType, ResourceType, float, CurrentValue, float, MaxValue);

/**
 * Component that manages character resources (HP, Mana, Stamina)
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMORPG_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceComponent();

	virtual void BeginPlay() override;

	// Get current value of a resource
	UFUNCTION(BlueprintCallable, Category = "Resources")
	float GetCurrentResource(EResourceType ResourceType) const;

	// Get max value of a resource
	UFUNCTION(BlueprintCallable, Category = "Resources")
	float GetMaxResource(EResourceType ResourceType) const;

	// Consume resource - returns true if successful
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool ConsumeResource(EResourceType ResourceType, float Amount);

	// Restore resource
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void RestoreResource(EResourceType ResourceType, float Amount);

	// Check if enough resource is available
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool HasEnoughResource(EResourceType ResourceType, float Amount) const;

	// Check if enough resources are available for multiple costs
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool HasEnoughResources(const TArray<FResourceCost>& Costs) const;

	// Consume multiple resources - returns true if all successful
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool ConsumeResources(const TArray<FResourceCost>& Costs);

	// Event fired when any resource changes
	UPROPERTY(BlueprintAssignable, Category = "Resources")
	FOnResourceChanged OnResourceChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float CurrentStamina;

	// Helper function to get resource reference
	float* GetResourceRef(EResourceType ResourceType);
	const float* GetResourceRef(EResourceType ResourceType) const;
	float* GetMaxResourceRef(EResourceType ResourceType);
	const float* GetMaxResourceRef(EResourceType ResourceType) const;
};
