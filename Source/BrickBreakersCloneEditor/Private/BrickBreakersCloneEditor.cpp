#include "BrickBreakersCloneEditor.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "ToolMenuEntry.h"
#include "LevelEditor/SLevelEditorTool.h"



#define LOCTEXT_NAMESPACE "FBrickBreakersCloneEditorModule"

void FBrickBreakersCloneEditorModule::StartupModule()
{
	if(UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::Get()->RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBrickBreakersCloneEditorModule::RegisterMenus));
	}
}

void FBrickBreakersCloneEditorModule::ShutdownModule()
{
	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::Get()->UnregisterOwner(this);
	}
}

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