// Fill out your copyright notice in the Description page of Project Settings.

#include "MagTiles.h"
#include "MagTileComponent.h"


// Sets default values for this component's properties
UMagTileComponent::UMagTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FRuntimeMeshTangent> Tangents;
    TArray<FColor> VertexColors;
    TArray<FVector2D> TextureCoordinates;
    TArray<int32> Triangles;

    // First vertex
    Vertices.Add(FVector(0, 100, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    VertexColors.Add(FColor::White);
    TextureCoordinates.Add(FVector2D(0, 0));

    // Second vertex
    Vertices.Add(FVector(100, 100, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    VertexColors.Add(FColor::White);
    TextureCoordinates.Add(FVector2D(0, 1));

    // Third vertex
    Vertices.Add(FVector(100, 0, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    VertexColors.Add(FColor::White);
    TextureCoordinates.Add(FVector2D(1, 1));

    // Fourth vertex
    Vertices.Add(FVector(0, 0, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    VertexColors.Add(FColor::White);
    TextureCoordinates.Add(FVector2D(1, 0));

    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);
    Triangles.Add(0);
    Triangles.Add(2);
    Triangles.Add(3);

    // Same as the CreateMeshSection above except we now enable collision and set the section to infrequent updates
    // RuntimeMesh->CreateMeshSection(0, Vertices, Triangles, Normals, TextureCoordinates, VertexColors, Tangents, true, EUpdateFrequency::Infrequent);

}


// Called when the game starts
void UMagTileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMagTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

