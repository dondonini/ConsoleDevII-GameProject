// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ConsoleGameDevII.h"
#include "ConsoleGameDevIIGameMode.h"
#include "ConsoleGameDevIIHUD.h"
#include "RobberCharacterControllerClass.h"
#include "ConsoleGameDevIICharacter.h"

AConsoleGameDevIIGameMode::AConsoleGameDevIIGameMode()
	: Super()
{

	static ConstructorHelpers::FClassFinder<APawn>
	PlayerPawnClassFinder(TEXT("/Game/Blueprint/Player/RobberCharacter_BP"));
	if (PlayerPawnClassFinder.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}

	PlayerControllerClass = ARobberCharacterControllerClass::StaticClass();

	// use our custom HUD class
	HUDClass = AConsoleGameDevIIHUD::StaticClass();
}
