// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PNM_KFC : ModuleRules
{
	public PNM_KFC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore", "AIModule", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
