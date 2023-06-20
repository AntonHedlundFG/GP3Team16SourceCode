using UnrealBuildTool;

public class Resources : ModuleRules
{
	public Resources(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]{
			"Core", "CoreUObject", "Engine", "InputCore", "Paper2D",
			"Utilities", "GP3Team16", "Buildings", "Units"
		});

		PrivateDependencyModuleNames.AddRange(new string[]{  });
	}
}
