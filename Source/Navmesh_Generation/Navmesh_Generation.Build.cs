// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Navmesh_Generation : ModuleRules
{
	public Navmesh_Generation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "ProceduralMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
