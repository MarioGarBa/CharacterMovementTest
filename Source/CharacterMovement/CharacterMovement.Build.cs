// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterMovement : ModuleRules
{
	public CharacterMovement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
