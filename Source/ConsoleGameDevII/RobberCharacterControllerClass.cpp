// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "RobberCharacterClass.h"
#include "RobberCharacterControllerClass.h"

ARobberCharacterControllerClass::ARobberCharacterControllerClass()
{

}

void ARobberCharacterControllerClass::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (InventoryWidgetBP)
	{
		InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);
	}

	bIsInventoryOpen = false;
}

void ARobberCharacterControllerClass::HandleInventoryInput()
{
	ARobberCharacterClass* Char = Cast<ARobberCharacterClass>(GetPawn());
	if (InventoryWidgetRef)
	{
		if (bIsInventoryOpen)
		{
			//Mark the inventory as closed
			bIsInventoryOpen = false;

			//Remove it from the viewport
			InventoryWidgetRef->RemoveFromViewport();
		}
		else
		{
			//Mark the inventory as open
			bIsInventoryOpen = true;

			//Re-populate the ItemsArray
			InventoryWidgetRef->ItemsArray = Char->GetInventory();

			//Show the inventory
			InventoryWidgetRef->Show();
		}

	}
}




