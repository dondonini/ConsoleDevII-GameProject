// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "InventoryWidget.h"


void UInventoryWidget::SetItemsName(FString a)
{
	ItemsName = FText::FromString(a);
}

FText UInventoryWidget::GetItemName() const
{
	return ItemsName;
}


