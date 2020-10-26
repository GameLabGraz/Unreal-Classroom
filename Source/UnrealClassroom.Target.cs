// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealClassroomTarget : TargetRules
{
    public UnrealClassroomTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new[] {"UnrealClassroom"});
    }
}