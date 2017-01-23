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

	MaxSpeedDefault = 500.f;
	SprintSpeedModifier = 1.5f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	/*
	// Camera Shake
	FirstPersonCameraShake = UCameraShake::StaticClass()->GetDefaultObject<UCameraShake>();
	FirstPersonCameraShake->OscillationDuration = -1.0f; //negative value will run forever
	FirstPersonCameraShake->RotOscillation.Pitch.Amplitude = 1.0f;
	FirstPersonCameraShake->RotOscillation.Pitch.Frequency = 0.5f;
	FirstPersonCameraShake->RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	FirstPersonCameraShake->RotOscillation.Yaw.Amplitude = 1.0f;
	FirstPersonCameraShake->RotOscillation.Yaw.Frequency = 0.5f;
	FirstPersonCameraShake->RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	*/

	RaycastRange = 250.0f;

	Inventory.SetNum(MAX_INVENTORY_SLOTS);
}

// Called when the game starts or when spawned
void ARobberCharacterClass::BeginPlay()
{
	Super::BeginPlay();

	LastSeenItem = nullptr;

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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARobberCharacterClass::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARobberCharacterClass::OnStopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARobberCharacterClass::ToggleCrouch);

	//Pickup
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ARobberCharacterClass::PickupItem);

}

void ARobberCharacterClass::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedDefault;
		AddMovementInput(GetActorForwardVector(), Value);

		GetNetOwningPlayer()->PlayerController->ClientPlayCameraShake(HeadBob, 20.0f /*raise this to 20 for debug*/);
		
	}
}

void ARobberCharacterClass::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedDefault;
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

void ARobberCharacterClass::OnStartSprint()
{
	MaxSpeedDefault = MaxSpeedDefault * SprintSpeedModifier;
}

void ARobberCharacterClass::OnStopSprint()
{
	MaxSpeedDefault = DEFAULT_MAX_SPEED;
}

void ARobberCharacterClass::ToggleCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
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
		LastSeenItem->SetOutline(false);
	}
	if (Pickup && Pickup != nullptr)
	{
		LastSeenItem = Pickup;
		Pickup->SetOutline(true);
	}
	else
		Pickup = nullptr;
}

void ARobberCharacterClass::PickupItem()
{
	//so if we are looking at an item 
	if (LastSeenItem)
	{
		int32 AvailableSlots = Inventory.Find(nullptr);

		if (AvailableSlots != INDEX_NONE)
		{
			Inventory[AvailableSlots] = LastSeenItem;

			//will change to equipping later on
			LastSeenItem->Destroy();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Cant Pick up anymore Items bitch"));
			//Add noise effects for cant pick up more item
		}
	}
}

void ARobberCharacterClass::HandleInventory()
{
	ARobberCharacterControllerClass* controller = Cast<ARobberCharacterControllerClass>(GetController());

	if (controller)
	{
		controller->HandleInventoryInput();
	}
}

