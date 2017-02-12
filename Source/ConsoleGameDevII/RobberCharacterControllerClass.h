// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "BinocularsWidgetClass.h"
#include "ConsoleGameDevIIHUD.h"
#include "RobberCharacterControllerClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API ARobberCharacterControllerClass : public APlayerController
{
	GENERATED_BODY()

	ARobberCharacterControllerClass();

	virtual void BeginPlay() override;

private:
	/*InventoryWidget reference*/
	UInventoryWidget* InventoryWidgetRef;

	UTootlipWidgetClass* TooltipWidgetRef;

	UBinocularsWidgetClass* BinocularsWidgetRef;

	/*True if the inventory is currently open - false otherwise*/
	bool bIsInventoryOpen;

protected:
	/*InventoryWidget Blueprint reference*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTootlipWidgetClass> TooltipWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBinocularsWidgetClass> BinocularsWidgetBP;


public:
	virtual void Possess(APawn* InPawn) override;
	void HandleInventoryInput();

	void ToggleTooltipUIOn();
	void ToggleTooltipUIOff();

	void ToggleBinocularsWidgetOn();
	void ToggleBinocularsWidgetOff();


	void SetNameOfWidget(FString name);

	void SetDescriptionOfWidget(FString desc);

	void SetNameOfInventoryWidget(FString name);

	/** Returns a pointer to the shooter game hud. May return NULL. */
	AConsoleGameDevIIHUD* GetHud() const;

	bool bIsBinocularsOpen;


	
	
};
