// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class MagTiles : ModuleRules
    {
        public MagTiles(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicIncludePaths.AddRange(
                new string[] {
                    // ... add public include paths required here ...
                }
            );

            PrivateIncludePaths.AddRange(
                new string[] {
                    "Developer/MagTiles/Private",
                    // ... add other private include paths required here ...
                }
            );

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core", "CoreUObject", // Core includes
                    "ShaderCore", "RenderCore", "RHI", "RuntimeMeshComponent", // For RMC
                    "Engine" // For things like actor component
                }
            );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    // ... add private dependencies that you statically link with here ...
                }
            );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
                {
                    // ... add any modules that your module loads dynamically here ...
                }
            );
        }
    }
}
