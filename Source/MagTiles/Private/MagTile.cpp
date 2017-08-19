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
    
}

AMagTile::~AMagTile()
{
    // @TODO Release RuntimeMesh

    // @TODO Release cache and trigger update of neighboring tiles
    
    //    FRuntimeMeshVertexTypeRegistrationContainer::GetInstance().UnRegister(&VertexType::TypeInfo);
    
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
    uint32 GroupKey = ParentComponent ? ParentComponent->GetUniqueID() : -1;
    FMagTileGroup* MagTileGroup = MagTileCore.GetTileGroup(GroupKey);
    FMagTileGroup* PrevMagTileGroup = PrevGroupKey ? MagTileCore.GetTileGroup(PrevGroupKey) : nullptr;

    GLog->Log(FString("MagTiles: Group key is ") + FString::FromInt(GroupKey));

    // Remove from previous group (remove even prev group is same as new group)
    if (PrevMagTileGroup)
    {
        PrevMagTileGroup->Unregister(*this);
        if (PrevMagTileGroup->IsEmpty()) {
            MagTileCore.UnregisterTileGroup(PrevGroupKey);
        }
    }

    // Update previous group key
    PrevGroupKey = GroupKey;

    // Create a group if not found.
    if (!MagTileGroup) 
    {
        MagTileGroup = new FMagTileGroup();
        MagTileCore.RegisterTileGroup(GroupKey, *MagTileGroup);
    }

    // Add to the group
    MagTileGroup->Register(*this);
    
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

FMagTileGroup::FMagTileGroup() 
{
    GLog->Log("MagTiles: Created a new group.");
}

FMagTileGroup::~FMagTileGroup()
{
    GLog->Log("MagTiles: Removed a group.");
}

void FMagTileGroup::Register(const AMagTile& MagTile)
{
    GLog->Log("MagTiles: Registering a tile.");
    // @TODO Add to registration

    // Compute my location
    FIntVector LocKey = MagTile.GetLocKey();

    UE_LOG(MagTiles, Log, TEXT("MagTile's Location is determined."));

    UE_LOG(
        MagTiles,
        Log,
        TEXT("MagTile's Location is %s and LocKey is %s"), 
        *(MagTile.GetRootComponent()->GetRelativeTransform().GetLocation().ToString()),
        *(LocKey.ToString())
    );
}

void FMagTileGroup::Unregister(const AMagTile& MagTile)
{
    GLog->Log("MagTiles: Unregistering a tile.");
    // @TODO implement
}

FMagTileCore::FMagTileCore() {
    GLog->Log("MagTiles: Core initialized.");
}

FMagTileCore& FMagTileCore::GetInstance()
{
    static FMagTileCore Instance;
    return Instance;
}

FMagTileGroup* FMagTileCore::GetTileGroup(uint32 Key)
{
    FMagTileGroup* MagTileGroup = MagTileGroups.Find(Key);
    //if (!MagTileGroup)
    //{
    //    MagTileGroup = new FMagTileGroup();
    //    MagTileGroups.Add(Key, *MagTileGroup);
    //}
    return MagTileGroup;
}

void FMagTileCore::RegisterTileGroup(uint32 Key, const FMagTileGroup& MagTileGroup)
{
    MagTileGroups.Add(Key, MagTileGroup);
}

void FMagTileCore::UnregisterTileGroup(uint32 key)
{
    MagTileGroups.Remove(key);
}