using UnrealBuildTool;

public class BrickBreakersCloneEditor : ModuleRules
{
    public BrickBreakersCloneEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "BrickBreakersClone"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd", "Slate", "SlateCore", "EditorStyle", "EditorWidgets", "PropertyEditor", "ToolMenus", "LevelEditor", "AssetTools", "UnrealEd"
            }
        );
    }
}