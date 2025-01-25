#include "BrickBreakersCloneEditor.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "ToolMenuEntry.h"
#include "LevelEditor/SLevelEditorTool.h"



#define LOCTEXT_NAMESPACE "FBrickBreakersCloneEditorModule"

/**
 * @brief Initializes the Brick Breakers Clone Editor module during startup.
 *
 * Registers a startup callback to set up tool menus if the tool menu UI is enabled.
 * This method is called when the module is first loaded by Unreal Engine.
 *
 * @note Only registers menu callbacks if the tool menu UI is currently enabled
 * @see RegisterMenus()
 * @see UToolMenus::IsToolMenuUIEnabled()
 */
void FBrickBreakersCloneEditorModule::StartupModule()
{
	if(UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::Get()->RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBrickBreakersCloneEditorModule::RegisterMenus));
	}
}

/**
 * @brief Unregisters the module's tool menu owner during module shutdown.
 *
 * This method is called when the Brick Breakers Clone Editor module is being shut down.
 * It checks if the tool menu UI is enabled and, if so, unregisters the current module
 * as an owner from the tool menu system. This ensures proper cleanup of any menu
 * entries or extensions added during the module's startup.
 *
 * @note Only performs unregistration if the tool menu UI is currently enabled.
 * @see FBrickBreakersCloneEditorModule::StartupModule()
 */
void FBrickBreakersCloneEditorModule::ShutdownModule()
{
	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::Get()->UnregisterOwner(this);
	}
}

/**
 * @brief Registers a custom menu section for Brick Breaker Tools in the Level Editor's main menu.
 *
 * This method extends the Level Editor's Tools menu by adding a new section titled "Brick Breaker Tools".
 * It creates a menu entry that allows users to open the Level Editor Tool for Brick Breaker.
 *
 * @details The method performs the following actions:
 * - Creates a scoped owner for the tool menu
 * - Retrieves the Level Editor's main menu
 * - Adds a new section named "Brick Breaker Tools"
 * - Adds a menu entry with the label "Level Editor" that triggers the OpenLevelEditorTool method
 *
 * @note If the menu cannot be extended, the method will silently return without creating the section
 *
 * @see FBrickBreakersCloneEditorModule::OpenLevelEditorTool()
 */
void FBrickBreakersCloneEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	if (!Menu) return;

	FToolMenuSection& Section = Menu->AddSection("BrickBreakerTools", FText::FromString("Brick Breaker Tools"));
	Section.AddMenuEntry(
		"OpenLevelEditor",
		FText::FromString("Level Editor"),
		FText::FromString("Open the Level Editor Tool for Brick Breaker"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FBrickBreakersCloneEditorModule::OpenLevelEditorTool))
	);
}

/**
 * @brief Opens a custom Brick Breaker Level Editor tool window.
 *
 * Creates a new Slate window for the Brick Breaker Level Editor with specific properties:
 * - Fixed window title "Brick Breaker Level Editor"
 * - Window size of 500x400 pixels
 * - Maximize and minimize buttons disabled
 * - Content populated with an SLevelEditorTool widget
 *
 * The window is added to the Slate application, making it visible to the user.
 *
 * @note This method is part of the Unreal Engine editor extension for the Brick Breakers Clone project.
 */
void FBrickBreakersCloneEditorModule::OpenLevelEditorTool()
{
	TSharedRef<SWindow> LevelEditorWindow = SNew(SWindow)
		.Title(LOCTEXT("LevelEditorWindowTitle", "Brick Breaker Level Editor"))
		.ClientSize(FVector2D(500, 400))
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	LevelEditorWindow->SetContent(SNew(SLevelEditorTool));
	FSlateApplication::Get().AddWindow(LevelEditorWindow);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBrickBreakersCloneEditorModule, BrickBreakersCloneEditor)