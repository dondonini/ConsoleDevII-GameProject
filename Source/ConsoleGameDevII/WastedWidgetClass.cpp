// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "WastedWidgetClass.h"



void UWastedWidgetClass::ToggleOn()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWastedWidgetClass::ToggleOff()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
