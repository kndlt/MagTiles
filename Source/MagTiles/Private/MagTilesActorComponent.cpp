// Fill out your copyright notice in the Description page of Project Settings.

#include "MagTiles.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagTilesActorComponent.h"


// Sets default values for this component's properties
UMagTilesActorComponent::UMagTilesActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMagTilesActorComponent::BeginPlay()
{
	Super::BeginPlay();

    GLog->Log("BeginPlay in your Awesome Plugin Component has fired!");
	
}


// Called every frame
void UMagTilesActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

