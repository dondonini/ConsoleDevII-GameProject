// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BoxWidgetClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API UBoxWidgetClass : public UUserWidget
{
	GENERATED_BODY()
public:
	void ToggleOn();

	void ToggleOff();
	
	
};
