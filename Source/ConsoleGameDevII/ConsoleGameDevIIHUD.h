// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "TootlipWidgetClass.h"
#include "ConsoleGameDevIIHUD.generated.h"

UCLASS()
class AConsoleGameDevIIHUD : public AHUD
{
	GENERATED_BODY()

public:
	AConsoleGameDevIIHUD();

	virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTootlipWidgetClass> TooltipWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTootlipWidgetClass* TooltipWidget;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	void ToggleToolTipUIOn();

	void ToggleToolTipUIOff();

};

