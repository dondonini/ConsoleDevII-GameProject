// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "ConsoleGameState.h"

void AConsoleGameState::Pause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AConsoleGameState::Unpause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

