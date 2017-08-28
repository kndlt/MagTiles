// Fill out your copyright notice in the Description page of Project Settings.

#include "MagTiles.h"
#include "MagTile.h"



AMagTile::AMagTile() :
    PrevGroupKey(0)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Init RMC
    RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
    RootComponent = RuntimeMesh;
    
    // V1 One Group
    // Who is going to define the grid?
    // Each tile belongs to a group
    // with same grid size and same grid offset (epsilon testing)
    // @TODO MagTileGroups
    //     FRuntimeMeshVertexTypeRegistrationContainer::GetInstance().Register(&VertexType::TypeInfo);
    GLog->Log("MagTiles: Created MagTile.");
}

void AMagTile::BeginDestroy()
{
    // @TODO Release RuntimeMesh

    // @TODO Release cache and trigger update of neighboring tiles
    
    //    FRuntimeMeshVertexTypeRegistrationContainer::GetInstance().UnRegister(&VertexType::TypeInfo);
    GLog->Log("MagTiles: Beginning to delete a MagTile.");

    // Remove from previous group (remove even prev group is same as new group)
    FMagTileCore& MagTileCore = FMagTileCore::GetInstance();
    FMagTileGroup* PrevMagTileGroup = PrevGroupKey ? MagTileCore.FindTileGroup(PrevGroupKey) : nullptr;

    if (PrevMagTileGroup)
    {
        PrevMagTileGroup->Unregister(*this);
        MagTileCore.RegisterTileGroup(PrevGroupKey, *PrevMagTileGroup);
        if (PrevMagTileGroup->IsEmpty()) {
            MagTileCore.UnregisterTileGroup(PrevGroupKey);
        }
    }

}

// Called when the game starts or when spawned
void AMagTile::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void AMagTile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

/**
 * Construction script
 *
 * @TODO Recovery logic for inconsistent state.
 */
void AMagTile::OnConstruction(const FTransform& Transform)
{
    CreateTileMesh(200);
    
    FMagTileCore& MagTileCore = FMagTileCore::GetInstance();
    USceneComponent* RootComponent = this->GetRootComponent();
    USceneComponent* ParentComponent = RootComponent->GetAttachParent();
    // @TODO Why is this empty?
    FMagTileGroup* PrevMagTileGroup = PrevGroupKey ? MagTileCore.FindTileGroup(PrevGroupKey) : nullptr;

    // Remove from previous group (remove even prev group is same as new group)
    if (PrevMagTileGroup)
    {
        PrevMagTileGroup->Unregister(*this);
        MagTileCore.RegisterTileGroup(PrevGroupKey, *PrevMagTileGroup);
        if (PrevMagTileGroup->IsEmpty()) {
            MagTileCore.UnregisterTileGroup(PrevGroupKey);
        }
    }

    // Get new tile group.
    uint32 GroupKey = ParentComponent ? ParentComponent->GetUniqueID() : 0xffffffff;

    // @TODO Why is this removing a group?
    FMagTileGroup* MagTileGroup = MagTileCore.FindTileGroup(GroupKey);

    if (!MagTileGroup) {
        MagTileGroup = new FMagTileGroup();
    }

    GLog->Log(FString("MagTiles: Group key is ") + FString::FromInt(GroupKey));

    // Update previous group key
    PrevGroupKey = GroupKey;

    // Add to the group
    MagTileGroup->Register(*this);

    // Have to do this at the end. Why????
    MagTileCore.RegisterTileGroup(GroupKey, *MagTileGroup);
    
    UE_LOG(MagTiles, Log, TEXT("MagTileCore verification: %d"), MagTileCore.FindTileGroup(GroupKey)->Registration.Num());

    GLog->Log("MagTiles: Huh?");

    return;
}



void AMagTile::CreateTileMesh(float SideLength)
{
    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FRuntimeMeshTangent> Tangents;
    TArray<FVector2D> TextureCoordinates;
    TArray<int32> Triangles;
    // First vertex
    Vertices.Add(FVector(0, SideLength, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    TextureCoordinates.Add(FVector2D(0, 0));

    // Second vertex
    Vertices.Add(FVector(SideLength, SideLength, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    TextureCoordinates.Add(FVector2D(0, 1));

    // Third vertex
    Vertices.Add(FVector(SideLength, 0, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    TextureCoordinates.Add(FVector2D(1, 1));

    // Fourth vertex
    Vertices.Add(FVector(0, 0, 0));
    Normals.Add(FVector(0, 0, 1));
    Tangents.Add(FRuntimeMeshTangent(0, -1, 0));
    TextureCoordinates.Add(FVector2D(1, 0));

    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);
    Triangles.Add(0);
    Triangles.Add(2);
    Triangles.Add(3);

    // Create the mesh section specifying collision
    RuntimeMesh->CreateMeshSection(0, Vertices, Triangles, Normals, TextureCoordinates, TArray<FColor>(), Tangents, true, EUpdateFrequency::Infrequent);
}

void FMagTileGroup::Register(const AMagTile& MagTile)
{
    GLog->Log("MagTiles: Registering a tile.");

    // Compute my location
    FIntVector LocKey = MagTile.GetLocKey();

    UE_LOG(MagTiles, Log, TEXT("MagTile's Location is %s and LocKey is %s"), *(MagTile.GetRootComponent()->GetRelativeTransform().GetLocation().ToString()), *(LocKey.ToString()));
    
    FMagTileNode* MagTileHead = Registration.Find(LocKey);

    float NewTileHeight = MagTile.GetRootComponent()->GetRelativeTransform().GetLocation().Z;

    // No previous elements
    if (!MagTileHead)
    {
        FMagTileNode* NewMagTileHead = new FMagTileNode(MagTile.GetUniqueID(), NewTileHeight, nullptr);
        Registration.Add(LocKey, *NewMagTileHead);
    }
    // Max heap push
    else {
        FMagTileNode* MagTileCursor = MagTileHead;
        FMagTileNode* MagTilePrev = nullptr;
        while (MagTileCursor) {
            float CursorTileHeight = MagTileCursor->height;
            
            if (NewTileHeight >= CursorTileHeight) {
                if (MagTilePrev) {
                    MagTilePrev->next = new FMagTileNode(MagTile.GetUniqueID(), NewTileHeight, MagTileCursor);
                }
                else {
                    MagTileHead = new FMagTileNode(MagTile.GetUniqueID(), NewTileHeight, MagTileCursor);
                }
                // This is very important!
                break;
            }
            MagTilePrev = MagTileCursor;
            MagTileCursor = MagTileCursor->next;
        }
        Registration.Add(LocKey, *MagTileHead);
    }

    RegistrationKeys.Add(MagTile.GetUniqueID(), LocKey);
}

void FMagTileGroup::Unregister(const AMagTile& MagTile)
{
    GLog->Log("MagTiles: Unregistering a tile.");

    // Find my previously assigned key
    FIntVector* LocKey = RegistrationKeys.Find(MagTile.GetUniqueID());

    if (LocKey)
    {
        // Get top most tile
        FMagTileNode* MagTileHead = Registration.Find(*LocKey);

        // Remove
        FMagTileNode* MagTileCursor = MagTileHead;
        FMagTileNode* MagTilePrev = nullptr;
        while (MagTileCursor) {
            // Found
            if (MagTileCursor->value == MagTile.GetUniqueID()) {
                if (MagTilePrev) {
                    MagTilePrev->next = MagTileCursor->next;
                }
                else {
                    MagTileHead = MagTileCursor->next;
                }
                break;
            }
            MagTilePrev = MagTileCursor;
            MagTileCursor = MagTileCursor->next;
        }

        // If any remaining, update the head.
        if (MagTileHead) {
            Registration.Add(*LocKey, *MagTileHead);
        }
        else {
            Registration.Remove(*LocKey);
        }
        RegistrationKeys.Remove(MagTile.GetUniqueID());
    }
}

