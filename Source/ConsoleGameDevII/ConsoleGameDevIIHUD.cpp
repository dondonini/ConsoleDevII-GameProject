// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ConsoleGameDevII.h"
#include "ConsoleGameDevIIHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "RobberCharacterClass.h"
#include "RobberCharacterControllerClass.h"
#include "CanvasItem.h"

AConsoleGameDevIIHUD::AConsoleGameDevIIHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshiarTexObj.Object;
}

void AConsoleGameDevIIHUD::BeginPlay()
{
	Super::BeginPlay();

	if (TooltipWidgetClass)
	{
		ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetOwner());
		TooltipWidget = CreateWidget<UTootlipWidgetClass>(MyController, TooltipWidgetClass);
		TooltipWidget->AddToViewport();
		TooltipWidget->SetVisibility(ESlateVisibility::Visible);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Created Tooltip Widget"));
	}
}

void AConsoleGameDevIIHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}



