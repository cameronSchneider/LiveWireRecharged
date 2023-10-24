// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LiveWireRecharged : ModuleRules
{
	public LiveWireRecharged(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
