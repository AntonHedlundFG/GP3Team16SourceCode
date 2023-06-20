using UnrealBuildTool;

public class Player : ModuleRules
{
	public Player(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"Utilities", "Buildings", "GP3Team16", "Units", "Buildings"
		});

		PrivateDependencyModuleNames.AddRange(new string[]{  });
	}
}
