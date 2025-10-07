// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceNodeActor.h"
#include "TimerManager.h"

AResourceNodeActor::AResourceNodeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a simple root component
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void AResourceNodeActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Start regeneration timer if enabled
	if (bRegenerates && RegenerationInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(
			RegenerationTimerHandle,
			this,
			&AResourceNodeActor::RegenerateResources,
			RegenerationInterval,
			true
		);
	}
}

int32 AResourceNodeActor::GatherResource(int32 AmountToGather)
{
	if (!HasResourcesAvailable() || AmountToGather <= 0)
	{
		return 0;
	}

	// Calculate the actual amount that can be gathered
	int32 ActualAmount = FMath::Min(AmountToGather, CurrentQuantity);
	
	// Reduce the current quantity
	CurrentQuantity -= ActualAmount;

	return ActualAmount;
}

bool AResourceNodeActor::HasResourcesAvailable() const
{
	return CurrentQuantity > 0;
}

void AResourceNodeActor::RegenerateResources()
{
	if (CurrentQuantity < MaxQuantity)
	{
		CurrentQuantity = FMath::Min(CurrentQuantity + RegenerationAmount, MaxQuantity);
	}
}
