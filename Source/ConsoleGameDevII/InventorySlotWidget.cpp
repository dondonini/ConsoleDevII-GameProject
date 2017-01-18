// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "InventorySlotWidget.h"

void UInventorySlotWidget::SetEquippedItem()
{
	//Empty for now
}

void UInventorySlotWidget::SetItemTexture(ABasePickupClass* Item)
{
	//If the item is valid update the widget's texture.
	//If not, assign a null ptr to it so the widget won't broadcast wrong information to the player
	(Item) ? ItemTexture = Item->GetPickUpTexture() : ItemTexture = nullptr;
}


