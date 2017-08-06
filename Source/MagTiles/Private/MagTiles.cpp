// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MagTiles.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IMagTiles.h"

DEFINE_LOG_CATEGORY(MagTiles);

class FMagTiles : public IMagTiles
{
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    bool IsThisNumber42(int32 num);
};

IMPLEMENT_MODULE( FMagTiles, MagTiles )

void FMagTiles::StartupModule()
{
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
    if (IMagTiles::IsAvailable())
    {
        UE_LOG(MagTiles, Log, TEXT("%s"), IMagTiles::Get().IsThisNumber42(42) ? TEXT("True") : TEXT("False"));
        UE_LOG(MagTiles, Log, TEXT("%s"), IMagTiles::Get().IsThisNumber42(12) ? TEXT("True") : TEXT("False"));
    }
}


void FMagTiles::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

bool FMagTiles::IsThisNumber42(int32 num)
{
    return num == 42;
}


