// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "RobberCharacterClass.h"
#include "RobberCharacterControllerClass.h"

ARobberCharacterControllerClass::ARobberCharacterControllerClass()
{

}

void ARobberCharacterControllerClass::BeginPlay()
{
	Super::BeginPlay();
	 
}

void ARobberCharacterControllerClass::HandleInventoryInput()
{
	ARobberCharacterClass* Char = Cast<ARobberCharacterClass>(GetPawn());
	InventoryWidgetRef->ItemsArray = Char->GetInventory();
	InventoryWidgetRef->Show();
}

void ARobberCharacterControllerClass::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (InventoryWidgetBP)
	{
		InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);

		InventoryWidgetRef->Show();

		ARobberCharacterClass* Char = Cast<ARobberCharacterClass>(GetPawn());

		InventoryWidgetRef->ItemsArray = Char->GetInventory();
	}

}







