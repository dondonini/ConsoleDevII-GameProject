// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WastedWidgetClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API UWastedWidgetClass : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ToggleOn();

	void ToggleOff();

	
};
