// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "BasePickupClass.h"


// Sets default values
ABasePickupClass::ABasePickupClass()
{
	PickupSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupSM"));

	PickupTexture = CreateDefaultSubobject<UTexture2D>(TEXT("Texture"));
}

void ABasePickupClass::SetOutline(bool Status)
{
	PickupSM->SetRenderCustomDepth(Status);
}


