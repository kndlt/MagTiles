// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagTile.generated.h"

UCLASS()
class MAGTILES_API AMagTile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMagTile();
    
    UPROPERTY(EditAnywhere)
    URuntimeMeshComponent* RuntimeMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    void OnConstruction(const FTransform& Transform) override;

    // Helper to generate a tile mesh
    void CreateTileMesh(float SideLength);
	
	
};
