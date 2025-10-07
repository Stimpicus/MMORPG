#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "HexSurfaceActor.generated.h"

UCLASS()
class AHexSurfaceActor : public AActor
{
	GENERATED_BODY()

public:
	AHexSurfaceActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hex")
	float SurfaceArea = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hex")
	float Depth = 1.f;

	// Utility: Calculates the hexagon side length given the area
	float GetHexSideLength(float Area) const;

	// Generates the mesh
	void GenerateHexMesh();
};