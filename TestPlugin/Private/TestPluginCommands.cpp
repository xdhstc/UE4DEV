// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TestPluginCommands.h"

#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FTestPluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "TestPlugin", "Execute TestPlugin action", EUserInterfaceActionType::Button, FInputGesture());

	UI_COMMAND(AnimControllAction, "animation controller", "Execute animation controller action", EUserInterfaceActionType::Button, FInputGesture());


	


}

#undef LOCTEXT_NAMESPACE
