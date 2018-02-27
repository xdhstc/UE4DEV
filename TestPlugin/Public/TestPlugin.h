// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/Slate/Public/Widgets/Docking/SDockTab.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
class FToolBarBuilder;
class FMenuBuilder;

class FTestPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
	void animationControllerClicked();
	
	TSharedRef<SDockTab> CreateTab(const FSpawnTabArgs& args);

	//TSharedRef<SDockTab> CreateTab1(const FSpawnTabArgs& args);
private:

	static void AddMenubarExtension(FMenuBarBuilder& Builder, const TSharedRef< FExtender > Extender);

	static void FillPulgMenu(FMenuBuilder& MenuBuilder, const TSharedRef< FExtender > Extender);



private:
	TSharedPtr<class FUICommandList> PluginCommands;
};