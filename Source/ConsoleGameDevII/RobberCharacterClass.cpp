// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "RobberCharacterClass.h"
#include "RobberCharacterControllerClass.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>


// Sets default values
ARobberCharacterClass::ARobberCharacterClass()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	RaycastRange = 250.0f;

	Inventory.SetNum(MAX_INVENTORY_SLOTS);

	EquippedItem = nullptr;

	currentInventoryIndex = 0;
}

// Called when the game starts or when spawned
void ARobberCharacterClass::BeginPlay()
{
	Super::BeginPlay();

	LastSeenItem = nullptr;

	ARobberCharacterControllerClass* controller = Cast<ARobberCharacterControllerClass>(GetController());
	if (controller)
	{
		controller->Possess(this);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Possessing"));
	}

}

// Called every frame
void ARobberCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RaycastForward();
}

// Called to bind functionality to input
void ARobberCharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARobberCharacterClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARobberCharacterClass::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARobberCharacterClass::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARobberCharacterClass::LookUpAtRate);

	//Pickup
	PlayerInputComponent->BindAction("Pickup",IE_Pressed, this, &ARobberCharacterClass::PickupItem);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ARobberCharacterClass::NextItem);
}

void ARobberCharacterClass::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ARobberCharacterClass::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ARobberCharacterClass::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARobberCharacterClass::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARobberCharacterClass::RaycastForward()
{
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * RaycastRange);

	FHitResult RaycastHit;

	//ignores self, shouldnt be a problem because its first person and theres no mesh really
	FCollisionQueryParams CPQ;
	CPQ.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CPQ);
	//DrawRay
	ABasePickupClass* Pickup = Cast<ABasePickupClass>(RaycastHit.GetActor());

	if (LastSeenItem && LastSeenItem != Pickup)
	{
		//If our character sees a different pickup then disable the glowing effect on the previous seen item
		LastSeenItem->SetOutline(false);
	}

	if (Pickup)
	{
		//Enable the glow effect on the current item
		LastSeenItem = Pickup;
		Pickup->SetOutline(true);
	}//Re-Initialize 
	else 
		LastSeenItem = nullptr;
}

void ARobberCharacterClass::PickupItem()
{
	//so if we are looking at an item 
	if (LastSeenItem && EquippedItem != LastSeenItem)
	{
		int32 AvailableSlots = Inventory.Find(nullptr);

		if (AvailableSlots != INDEX_NONE)
		{
			Inventory[AvailableSlots] = LastSeenItem;

			ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());
			if (MyController)
			{
				MyController->HandleInventoryInput();
			}

			//is this our first item?
			if (EquippedItem == nullptr)
			{
				FVector EquippedVector = (FirstPersonCameraComponent->GetComponentLocation() + EquipmentVectorOffset);
				EquippedItem = Inventory[AvailableSlots];

				FVector SocketLocationR;
				SocketLocationR = GetMesh()->GetSocketLocation("GripPoint");
				EquippedItem->AttachRootComponentTo(GetMesh(), FName(TEXT("GripPoint")), EAttachLocation::SnapToTarget);
				EquippedItem->SetActorEnableCollision(false);
				currentInventoryIndex = 0;
			}

			else
			{
				FVector SocketLocationR;
				SocketLocationR = GetMesh()->GetSocketLocation("GripPoint");
				Inventory[AvailableSlots]->AttachRootComponentTo(GetMesh(), FName(TEXT("GripPoint")), EAttachLocation::SnapToTarget);
				Inventory[AvailableSlots]->SetActorEnableCollision(false);
				Inventory[AvailableSlots]->SetActorHiddenInGame(true);
				//EquippedItem->SetActorLocation(FirstPersonCameraComponent->GetComponentLocation());
				//Inventory[AvailableSlots]->SetActorHiddenInGame(true);
			}

			//Debugging
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,EquippedItem->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(AvailableSlots));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Cant Pick up anymore Items bitch"));
			//Add noise effects for cant pick up more item
		}
	}

	
}

void ARobberCharacterClass::NextItem()
{
	Inventory[currentInventoryIndex]->SetActorHiddenInGame(true);
	currentInventoryIndex++;
	if (Inventory[currentInventoryIndex])
	{
		EquippedItem = Inventory[currentInventoryIndex];
		EquippedItem->SetActorHiddenInGame(false);
	}
}

void ARobberCharacterClass::SetEquippedItem(UTexture2D* Texture)
{
	if (Texture)
	{

	}
}


