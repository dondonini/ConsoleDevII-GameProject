// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "TootlipWidgetClass.h"



void UTootlipWidgetClass::ToggleOn()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UTootlipWidgetClass::ToggleOff()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

FText UTootlipWidgetClass::GetItemName() const 
{
	return ActorName;
}

FText UTootlipWidgetClass::GetItemDescription() const
{
	return description;
}

void UTootlipWidgetClass::SetName(FString a)
{
	ActorName = FText::FromString(a);
}

void UTootlipWidgetClass::SetDescription(FString b)
{
	description = FText::FromString(b);
}



