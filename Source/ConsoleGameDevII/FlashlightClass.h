// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePickupClass.h"
#include "FlashlightClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API AFlashlightClass : public ABasePickupClass
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	bool bIsActive = false;

	UFUNCTION(BlueprintCallable, Category = Info)
	bool GetStatus();
	
	
};
