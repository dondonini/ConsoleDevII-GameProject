// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TootlipWidgetClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API UTootlipWidgetClass : public UUserWidget
{
	GENERATED_BODY()

	

public:
	void ToggleOn();

	void ToggleOff();
	
	FText ActorName;

	FText description;

	void SetName(FString a);

	void SetDescription(FString b);

	UFUNCTION(BlueprintCallable, Category = Information)
	FText GetItemName() const;

	UFUNCTION(BlueprintCallable, Category = Information)
	FText GetItemDescription() const;
	
	
};
