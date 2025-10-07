// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * Base class for all items in the game
 * Provides core properties like name, description, weight, value
 */
UCLASS(Blueprintable, BlueprintType)
class MMORPG_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 CurrentStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;

	// Virtual functions for extensibility
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual FString GetItemInfo() const;
};
