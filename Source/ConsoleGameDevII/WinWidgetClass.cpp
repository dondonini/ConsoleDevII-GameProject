// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "WinWidgetClass.h"


void UWinWidgetClass::ToggleOn()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWinWidgetClass::ToggleOff()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}


