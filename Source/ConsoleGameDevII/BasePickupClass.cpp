// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "BasePickupClass.h"


// Sets default values
ABasePickupClass::ABasePickupClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasePickupClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePickupClass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

