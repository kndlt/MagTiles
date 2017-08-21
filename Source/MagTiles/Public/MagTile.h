// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagTile.generated.h"

UCLASS()
class MAGTILES_API AMagTile : public AActor
{
	GENERATED_BODY()

    // Prev Group Key
    uint32 PrevGroupKey;

    void OnConstruction(const FTransform& Transform) override;

    // Helper to generate a tile mesh
    void CreateTileMesh(float SideLength);

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	

    UPROPERTY(EditAnywhere)
    URuntimeMeshComponent* RuntimeMesh;

    FIntVector GetLocKey() const {
        USceneComponent* RootComponent = this->GetRootComponent();
        FVector Location = RootComponent->GetRelativeTransform().GetLocation();
        // @TODO get int size arguments.
        int32 size = 200;
        float sizeInFloat = size;
        // i, j, size
        FIntVector LocKey(
            (size != 0 ? (int) (Location.X / sizeInFloat + 0.5f) : 0),
            (size != 0 ? (int) (Location.Y / sizeInFloat + 0.5f) : 0),
            size
        );
        return LocKey;
    }

    AMagTile();
    
    ~AMagTile();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

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
 * Linked list node for MagTiles
 */
USTRUCT()
struct MAGTILES_API FMagTileNode
{
    GENERATED_USTRUCT_BODY()


    // @TODO Need to use uint32 UniqueID
    UPROPERTY()
    const AMagTile* value;

    FMagTileNode* next;

    FMagTileNode();
    FMagTileNode(const AMagTile* value, FMagTileNode* next = nullptr);
    ~FMagTileNode();
};

/**
 * Tile groups
 */
USTRUCT()
struct MAGTILES_API FMagTileGroup
{
    GENERATED_USTRUCT_BODY()

    //  tiles <<row, col, size>, tile linked list> (assume size is int for V1)
    UPROPERTY()
    TMap<FIntVector, FMagTileNode> Registration;

    // GUID -> Keys
    UPROPERTY()
    TMap<uint32, FIntVector> RegistrationKeys;

    FMagTileGroup();
    
    ~FMagTileGroup();
    
    // static FRuntimeMeshVertexTypeRegistrationContainer& GetInstance();
    
    void Register(const AMagTile& MagTile);
    
    void Unregister(const AMagTile& MagTile);

    bool IsEmpty() {
        return Registration.Num() == 0;
    }
    
    // const FRuntimeMeshVertexTypeInfo* GetVertexType(FGuid Key) const;
    
};

/**
 * MagTile Global
 */
USTRUCT()
struct MAGTILES_API FMagTileCore
{
    GENERATED_BODY()
    
    // Parent ID -> group
    UPROPERTY()
    TMap<uint32, FMagTileGroup> MagTileGroups;
    
    static FMagTileCore& GetInstance();

    FMagTileCore();

    ~FMagTileCore();
    
    FMagTileGroup* GetTileGroup(uint32 Key);

    void RegisterTileGroup(uint32 Key, const FMagTileGroup& MagTileGroup);

    void UnregisterTileGroup(uint32 Key);
    
    // Automatically called when a new MagTile (first out of all siblings)
    // is registered.
    // Register()
    
    // Unregistered when Reference count hits zero.
    // Unregister()
};



