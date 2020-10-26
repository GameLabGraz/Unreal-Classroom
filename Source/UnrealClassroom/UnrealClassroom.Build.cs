// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealClassroom : ModuleRules
{
    public UnrealClassroom(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[] {"Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem"});

        PrivateDependencyModuleNames.AddRange(new[] {"HeadMountedDisplay"});

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}