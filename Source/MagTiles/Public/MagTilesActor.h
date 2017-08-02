// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagTilesActorComponent.h"
#include "MagTilesActor.generated.h"

UCLASS()
class MAGTILES_API AMagTilesActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagTilesActor();

protected:

    UPROPERTY(VisibleAnywhere)
    UMagTilesActorComponent* MagTilesActorComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
