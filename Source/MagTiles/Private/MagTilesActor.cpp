// Fill out your copyright notice in the Description page of Project Settings.

#include "MagTiles.h"
#include "MagTilesActor.h"


// Sets default values
AMagTilesActor::AMagTilesActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MagTilesActorComponent = CreateDefaultSubobject<UMagTilesActorComponent>(FName("MagTilesActorComponent"));
}

// Called when the game starts or when spawned
void AMagTilesActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagTilesActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

