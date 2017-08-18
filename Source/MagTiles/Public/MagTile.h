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

    UPROPERTY(EditAnywhere)
    
    URuntimeMeshComponent* RuntimeMesh;

    AMagTile();
    
    ~AMagTile();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void OnConstruction(const FTransform& Transform) override;

    // Helper to generate a tile mesh
    void CreateTileMesh(float SideLength);
	
	
};


// MagTiles look 1 level up in the hiearchy to see if there are any similar size tiles.
// No rotation is supported, no scaling is supported for individual tiles, but as a hiearchy yes.
// Any MagTiles which are siblings will be somehow linked together.
// Most likely is that there will be a common registry for the GUID,
// So in C++ code we need a map <FGuid, FMagTileGroup>
// and each FMagTileGroup will have a tile registry of map <<int,int>,<AMagTile>>.
// if the tile does not conform to the location, we can either SNAP, or mark it red.
// I like the idea of dimensional snapping.
// All grid will have no offset as parent element can be trnasformed.




/**
 * Tile index location
 */
//struct MAGTILES_API FMagTileLocation
//{
//    const int64 i, j;
//};

/**
 * Linked list node for MagTiles
 */
//struct MAGTILES_API FMagTileNode
//{
//    const AMagTile* value;
//    const FMagTileNode* next;
//};

/**
 * Tile groups
 */
class MAGTILES_API FMagTileGroup
{
    // Vector of tiles
    TArray<AMagTile*> Registration;
    // TMap<FMagTileLocation, FMagTileNode> Registrations;

public:
    
    FMagTileGroup();
    
    ~FMagTileGroup();
    
    // static FRuntimeMeshVertexTypeRegistrationContainer& GetInstance();
    
    void Register(const AMagTile* MagTile);
    
    void Unregister(const AMagTile* MagTile);
    
    // const FRuntimeMeshVertexTypeInfo* GetVertexType(FGuid Key) const;
    
};

/**
 * MagTile Global
 */
class MAGTILES_API FMagTileCore
{
    
    // Parent ID -> group
    TMap<uint32, FMagTileGroup> MagTileGroups;
    
public:
    
    static FMagTileCore& GetInstance();
    
    FMagTileCore();
    
    FMagTileGroup& GetTileGroup(uint32 Key);
    
    // Automatically called when a new MagTile (first out of all siblings)
    // is registered.
    // Register()
    
    // Unregistered when Reference count hits zero.
    // Unregister()
};



