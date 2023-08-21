// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Navmesh_GenerationEditorTarget : TargetRules
{
	public Navmesh_GenerationEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
        ExtraModuleNames.AddRange( new string[] { "Navmesh_Generation" } );
	}
}
