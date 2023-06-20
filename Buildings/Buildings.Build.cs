using UnrealBuildTool;

public class Buildings : ModuleRules
{
	public Buildings(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]{
			"Core", "CoreUObject", "Engine", "InputCore", 
			"EnhancedInput", "Utilities", "Units", "Resources", "GP3Team16"
		});

		PrivateDependencyModuleNames.AddRange(new string[]{  });
	}
}
