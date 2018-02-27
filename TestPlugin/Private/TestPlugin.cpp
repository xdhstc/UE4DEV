// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestPlugin.h"
#include "TestPluginStyle.h"
#include "TestPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"
#include "RigControllerWindow.h"

static const FName TestPluginTabName("TestPlugin");

const FName RigControllerWindowTabName(TEXT("RigControllerWindowTab"));


#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FTestPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestPluginStyle::Initialize();
	FTestPluginStyle::ReloadTextures();

	FTestPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);



	PluginCommands->MapAction(
		FTestPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FTestPluginModule::PluginButtonClicked),
		FCanExecuteAction());


	PluginCommands->MapAction(
		FTestPluginCommands::Get().AnimControllAction,
		FExecuteAction::CreateRaw(this, &FTestPluginModule::animationControllerClicked),
		FCanExecuteAction());




	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> MenubarExtender = MakeShareable(new FExtender);

	MenubarExtender->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands, FMenuBarExtensionDelegate::CreateStatic(&FTestPluginModule::AddMenubarExtension, MenubarExtender.ToSharedRef()));


	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenubarExtender);
	FGlobalTabmanager::Get()->RegisterTabSpawner(RigControllerWindowTabName, FOnSpawnTab::CreateRaw(this, &FTestPluginModule::CreateTab))
		.SetDisplayName(FText::FromString(TEXT("rig controller")));



}

void FTestPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTestPluginStyle::Shutdown();

	FTestPluginCommands::Unregister();
}

void FTestPluginModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "12443Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FTestPluginModule::PluginButtonClicked()")),
							FText::FromString(TEXT("TestPlugin.cpp"))
					   ); 
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FTestPluginModule::animationControllerClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("animation controller click"));
	FGlobalTabmanager::Get()->InvokeTab(RigControllerWindowTabName);



	/*
	TSharedRef<SWindow> cookbookwin = SNew(SWindow)
		.Title(FText::FromString(TEXT("cookbook win")))
		.ClientSize(FVector2D(800,400))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("hello fuck")))
		]



		];
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(cookbookwin, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(cookbookwin);

	}
	*/
}

TSharedRef<SDockTab> FTestPluginModule::CreateTab(const FSpawnTabArgs & args)
{
	return 
		SNew(SDockTab)
		.Label(LOCTEXT("TabTitle","Rig Controller"))
		.TabRole(ETabRole::MajorTab)
		.ContentPadding(5)
		[
			SNew(SBorder)
			.Padding(4)
			//.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(RigControllerWindow)
			]
		];
}





void FTestPluginModule::AddMenubarExtension(FMenuBarBuilder & Builder, const TSharedRef< FExtender > Extender)
{
	Builder.AddPullDownMenu(
		LOCTEXT("ProjectMenu", "My Project"),
		LOCTEXT("ProjectMenu_ToolTip", "Project Plugin"),
		FNewMenuDelegate::CreateStatic(FTestPluginModule::FillPulgMenu, Extender),
		"PlugHook",
		FName(TEXT("ProjectMenu"))
	);
}

void FTestPluginModule::FillPulgMenu(FMenuBuilder& MenuBuilder, const TSharedRef< FExtender > Extender)
{

	MenuBuilder.BeginSection("Model", LOCTEXT("ModelHeading", "Model Tool"));
	{
		MenuBuilder.AddMenuEntry(FTestPluginCommands::Get().PluginAction);
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("Animation", LOCTEXT("AnimationHeading", "Animation Tool"));
	{
		MenuBuilder.AddMenuEntry(FTestPluginCommands::Get().AnimControllAction);
	}
	MenuBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestPluginModule, TestPlugin)