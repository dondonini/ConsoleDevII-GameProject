// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "ConsoleGameState.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API AConsoleGameState : public AGameState
{
	GENERATED_BODY()
public:
	void Pause();

	void Unpause();
	
	
};
