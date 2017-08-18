// Fill out your copyright notice in the Description page of Project Settings.

#include "MagTiles.h"
#include "MagTile.h"



AMagTile::AMagTile()
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

void AMagTile::OnConstruction(const FTransform& Transform)
{
    uint32 GroupKey;

    CreateTileMesh(200);
    
    FMagTileCore& MagTileCore = FMagTileCore::GetInstance();
    
    // @TODO Get the name of parent
    USceneComponent* RootComponent = this->GetRootComponent();
    USceneComponent* ParentComponent = RootComponent->GetAttachParent();
    
    if (ParentComponent) {
        GroupKey = ParentComponent->GetUniqueID();
    }
    else {
        GroupKey = 0;
    }

    GLog->Log(FString("MagTiles: Group key is ") + FString::FromInt(GroupKey));

    FMagTileGroup& MagTileGroup = MagTileCore.GetTileGroup(GroupKey);
    
    // Add self
    MagTileGroup.Register(this);
    
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

void FMagTileGroup::Register(const AMagTile* MagTile)
{
    GLog->Log("MagTiles: Registering a tile.");
    check(MagTile);
    // @TODO Add to registration
}

void FMagTileGroup::Unregister(const AMagTile* MagTile)
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

FMagTileGroup& FMagTileCore::GetTileGroup(uint32 Key)
{
    // return *(new FMagTileGroup());
    FMagTileGroup* MagTileGroup = MagTileGroups.Find(Key);
    if (MagTileGroup == nullptr)
    {
        MagTileGroup = new FMagTileGroup();
        
        // @TODO Fix error on this line MemoryOps
        // MagTileGroups.Add(Key, *MagTileGroup);
    }
    return *MagTileGroup;
    //return *(new FMagTileGroup());
}
