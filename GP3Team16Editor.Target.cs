using UnrealBuildTool;
using System.Collections.Generic;

public class GP3Team16EditorTarget : TargetRules
{
	public GP3Team16EditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("GP3Team16");
		ExtraModuleNames.Add("Player");
        ExtraModuleNames.Add("Utilities");
        ExtraModuleNames.Add("Units");
        ExtraModuleNames.Add("Buildings");
        ExtraModuleNames.Add("Resources");
	}
}
