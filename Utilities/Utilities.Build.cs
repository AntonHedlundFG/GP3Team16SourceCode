using UnrealBuildTool;

public class Utilities : ModuleRules
{
	public Utilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]{
			"Core", "CoreUObject", "Engine", "InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]{  });
	}
}
