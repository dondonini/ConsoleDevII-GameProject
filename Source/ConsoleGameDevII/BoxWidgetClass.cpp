// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "BoxWidgetClass.h"


void UBoxWidgetClass::ToggleOn()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UBoxWidgetClass::ToggleOff()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

