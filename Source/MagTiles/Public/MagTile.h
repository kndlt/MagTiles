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
    uint32 PrevGroupKey = 0xffffffff;

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
    uint32 value;

    UPROPERTY()
    float height;

    FMagTileNode* next;

    // @TODO Default value does not make sense.
    FMagTileNode() :
        value(0),
        height(0),
        next(nullptr)
    {
        GLog->Log("MagTiles: Created a new node.");
    }

    FMagTileNode(const uint32 value, const float height, FMagTileNode* next = nullptr) :
        value(value),
        height(height),
        next(next)
    {
        GLog->Log("MagTiles: Created a new node.");
    }

    ~FMagTileNode()
    {
        GLog->Log("MagTiles: Deleted a node.");
    }
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

    FMagTileGroup()
    {
        GLog->Log("MagTiles: Created a new group.");
    }
    
    ~FMagTileGroup()
    {
        GLog->Log("MagTiles: Removed a group.");
    }
        
    void Register(const AMagTile& MagTile);
    
    void Unregister(const AMagTile& MagTile);

    bool IsEmpty() {
        return Registration.Num() == 0;
    }    
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

    static FMagTileCore& GetInstance()
    {
        static FMagTileCore Instance;
        return Instance;
    }

    FMagTileCore()
    {
        GLog->Log("MagTiles: Core initialized.");
    }

    ~FMagTileCore()
    {
        GLog->Log("MagTiles: Core destroyed.");
    }

    FMagTileGroup* FindTileGroup(uint32 Key) 
    {
        return MagTileGroups.Find(Key);
    }

    FMagTileGroup& FindOrAddTileGroup(uint32 Key)
    {
        return MagTileGroups.FindOrAdd(Key);
    }

    void RegisterTileGroup(uint32 Key, const FMagTileGroup& MagTileGroup)
    {
        GLog->Log("MagTiles: Tile group registered.");
        MagTileGroups.Add(Key, MagTileGroup);
    }

    void UnregisterTileGroup(uint32 Key)
    {
        GLog->Log("MagTiles: Tile group unregistered.");
        MagTileGroups.Remove(Key);
    }
    
    // Automatically called when a new MagTile (first out of all siblings)
    // is registered.
    // Register()
    
    // Unregistered when Reference count hits zero.
    // Unregister()
};
