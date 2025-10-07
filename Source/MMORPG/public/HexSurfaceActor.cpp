// Copyright Epic Games, Inc. All Rights Reserved.

#include "HexSurfaceActor.h"

AHexSurfaceActor::AHexSurfaceActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	SetRootComponent(ProceduralMesh);
	ProceduralMesh->SetMobility(EComponentMobility::Static);
}

void AHexSurfaceActor::OnConstruction(const FTransform& Transform)
{
	GenerateHexMesh();
}

float AHexSurfaceActor::GetHexSideLength(float Area) const
{
	// A = (3 * sqrt(3) / 2) * a^2
	const float HexConstant = 3.f * FMath::Sqrt(3.f) / 2.f;
	return FMath::Sqrt(Area / HexConstant);
}

void AHexSurfaceActor::GenerateHexMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	// Convert meters to centimeters for UE units
	const float SideCm = GetHexSideLength(SurfaceArea) * 100.0f; // Side in cm
	const float RadiusCm = SideCm; // For a regular hexagon, center-to-vertex = side length
	const float HalfDepthCm = Depth * 50.0f; // Depth is in meters; half-depth in cm

	// Hexagon vertices (top face, Z+)
	for (int i = 0; i < 6; ++i)
	{
		float AngleDeg = 60.f * i - 30.f;
		float AngleRad = FMath::DegreesToRadians(AngleDeg);
		Vertices.Add(FVector(
			RadiusCm * FMath::Cos(AngleRad),
			RadiusCm * FMath::Sin(AngleRad),
			HalfDepthCm // Top face at Z+
		));
	}
	// Hexagon vertices (bottom face, Z-)
	for (int i = 0; i < 6; ++i)
	{
		float AngleDeg = 60.f * i - 30.f;
		float AngleRad = FMath::DegreesToRadians(AngleDeg);
		Vertices.Add(FVector(
			RadiusCm * FMath::Cos(AngleRad),
			RadiusCm * FMath::Sin(AngleRad),
			-HalfDepthCm // Bottom face at Z-
		));
	}

	// Top face triangles (winding order)
	for (int i = 1; i < 5; ++i)
	{
		Triangles.Add(0);
		Triangles.Add(i);
		Triangles.Add(i + 1);
	}

	// Bottom face triangles (reverse winding)
	for (int i = 1; i < 5; ++i)
	{
		Triangles.Add(6);
		Triangles.Add(6 + i + 1);
		Triangles.Add(6 + i);
	}

	// Side faces (quads split into triangles)
	for (int i = 0; i < 6; ++i)
	{
		int TopA = i;
		int TopB = (i + 1) % 6;
		int BottomA = i + 6;
		int BottomB = ((i + 1) % 6) + 6;

		// First triangle
		Triangles.Add(TopA);
		Triangles.Add(BottomA);
		Triangles.Add(BottomB);

		// Second triangle
		Triangles.Add(TopA);
		Triangles.Add(BottomB);
		Triangles.Add(TopB);
	}

	// Simple normals (top: Z+, bottom: Z-, sides: not perfect)
	for (int i = 0; i < 12; ++i)
	{
		Normals.Add(FVector(0, 0, (i < 6) ? 1 : -1));
		UVs.Add(FVector2D(0.f, 0.f));
		Tangents.Add(FProcMeshTangent(1.f, 0.f, 0.f));
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, nullptr); // Assign material in editor/code if desired
}