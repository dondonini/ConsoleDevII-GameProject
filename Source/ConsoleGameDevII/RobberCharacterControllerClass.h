// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "RobberCharacterControllerClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API ARobberCharacterControllerClass : public APlayerController
{
	GENERATED_BODY()

	ARobberCharacterControllerClass();

private:
	/*InventoryWidget reference*/
	UInventoryWidget* InventoryWidgetRef;

	/*True if the inventory is currently open - false otherwise*/
	bool bIsInventoryOpen;

protected:
	/*InventoryWidget Blueprint reference*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetBP;


public:
	virtual void Possess(APawn* InPawn) override;
	void HandleInventoryInput();
	
	
};
