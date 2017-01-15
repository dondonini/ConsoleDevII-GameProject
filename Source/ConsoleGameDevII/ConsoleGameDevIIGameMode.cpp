// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ConsoleGameDevII.h"
#include "ConsoleGameDevIIGameMode.h"
#include "ConsoleGameDevIIHUD.h"
#include "ConsoleGameDevIICharacter.h"

AConsoleGameDevIIGameMode::AConsoleGameDevIIGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn>
	PlayerPawnClassFinder(TEXT("/Game/Blueprint/Player/RobberCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AConsoleGameDevIIHUD::StaticClass();
}
