using UnrealBuildTool;

public class Units : ModuleRules
{
	public Units(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]{
			"Core", "CoreUObject", "Engine", "InputCore", "Paper2D",
			"EnhancedInput", "Utilities", "GP3Team16", "Resources",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]{  });
	}
}
