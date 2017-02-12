// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "BinocularsWidgetClass.h"




void UBinocularsWidgetClass::ToggleOn()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UBinocularsWidgetClass::ToggleOff()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}