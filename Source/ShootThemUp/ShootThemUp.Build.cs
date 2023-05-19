// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
	public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Niagara",//粒子模块
			"PhysicsCore",//物理核心  物理材质需要
			"GameplayTasks",//游戏任务模块
			"NavigationSystem"//导航系统
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] 
		{ 
			"ShootThemUp/Public/Player",
			"ShootThemUp/Public/Components",
			"ShootThemUp/Public/Dev",
			"ShootThemUp/Public/Weapon",
			"ShootThemUp/Public/UI",
			"ShootThemUp/public/Animations",
			"ShootThemUp/public/Weapon/Components",
			"ShootThemUp/public/Player/AI",
			"ShootThemUp/public/Player/AI/Task",
			"ShootThemUp/public/Player/AI/Services",
			"ShootThemUp/public/Player/AI/EQS",
			"ShootThemUp/public/Player/AI/Decorators"
		});//公共包含的路径

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
