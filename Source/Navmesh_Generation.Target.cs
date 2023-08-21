// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Navmesh_GenerationTarget : TargetRules
{
	public Navmesh_GenerationTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
        ExtraModuleNames.AddRange( new string[] { "Navmesh_Generation" } );
	}
}
