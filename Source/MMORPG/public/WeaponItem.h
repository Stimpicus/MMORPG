// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatTypes.h"
#include "WeaponItem.generated.h"

/**
 * Base weapon item class
 */
UCLASS(Blueprintable)
class MMORPG_API AWeaponItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponItem();

	// Get weapon data
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FWeaponData GetWeaponData() const { return WeaponData; }

	// Set weapon data
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponData(const FWeaponData& NewWeaponData);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponData WeaponData;

	// Visual mesh component (optional)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
};
