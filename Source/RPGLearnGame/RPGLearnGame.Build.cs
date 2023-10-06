// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGLearnGame : ModuleRules
{
	public RPGLearnGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput"
		});
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"MotionWarping",
			"MotionTrajectory",
			"AnimGraphRuntime"
		});
	}
}
