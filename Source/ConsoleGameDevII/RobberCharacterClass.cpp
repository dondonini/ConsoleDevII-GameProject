// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "RobberCharacterClass.h"
#include "RobberCharacterControllerClass.h"
#include "HomeBaseClass.h"
#include "FlashlightClass.h"
#include "BinocularsClass.h"
#include "ObjectiveActor.h"
#include "BoxClass.h"
#include "LockpickClass.h"
#include "EnemyAI.h"
#include "Spawn.h"
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

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->AttachTo(RootComponent);

	//Cameraboom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->SocketOffset = SpringArmOffset;
	CameraBoom->TargetArmLength = SpringArmLength;
	CameraBoom->bUsePawnControlRotation = false;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraBoom);
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false;



	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);


	RaycastRange = 250.0f;

	Inventory.SetNum(MAX_INVENTORY_SLOTS);

	EquippedItem = nullptr;

	currentInventoryIndex = 0;

	EnemyTimer = 3.0f;

	bHidden = false;
}


// Called when the game starts or when spawned
void ARobberCharacterClass::BeginPlay()
{
	Super::BeginPlay();

	LastSeenItem = nullptr;

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARobberCharacterClass::OnBoxOverlap);

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
	RaycastItem();
	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());


	if (MyController->bIsBinocularsOpen && !bManualZoom)
	{
		BinocularsRaycast(DeltaTime);
		if (!FMath::IsNearlyEqual(CameraBoom->TargetArmLength, -1000.0f))
		{
			CameraBoom->TargetArmLength = (FMath::FInterpTo(CameraBoom->TargetArmLength, -1000.0f, DeltaTime, 25.f));
			Mesh1P->AttachTo(RootComponent);
		}
	}

	if (!MyController->bIsBinocularsOpen)
	{
		if (!FMath::IsNearlyEqual(CameraBoom->TargetArmLength, 0.0f))
		{
			CameraBoom->TargetArmLength = (FMath::FInterpTo(CameraBoom->TargetArmLength, 0.0f, DeltaTime, 25.f));
			Mesh1P->AttachTo(FirstPersonCameraComponent);
		}
	}
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
	PlayerInputComponent->BindAction("ItemToggle", IE_Pressed, this, &ARobberCharacterClass::ToggleItemFunctions);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARobberCharacterClass::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARobberCharacterClass::OnStopSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARobberCharacterClass::ToggleCrouch);

	//Binoculars Zoom
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARobberCharacterClass::BinocularsZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARobberCharacterClass::BinocularsZoomOut);

}

void ARobberCharacterClass::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedDefault;
		AddMovementInput(GetActorForwardVector(), Value);

		GetNetOwningPlayer()->PlayerController->ClientPlayCameraShake(HeadBob, 1.0f /*raise this to 20 for debug*/);
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

/*Toggle Tooltip UI*/
void ARobberCharacterClass::RaycastItem()
{
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * RaycastRange);

	FHitResult RaycastHit;

	//ignores self, shouldnt be a problem because its first person and theres no mesh really
	FCollisionQueryParams CPQ;
	CPQ.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CPQ);
	//DrawRay
	ABasePickupClass* Item = Cast<ABasePickupClass>(RaycastHit.GetActor());

	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());

	if (Item)
	{
		MyController->ToggleTooltipUIOn();
	}
	else
		MyController->ToggleTooltipUIOff();
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

	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());

	bool bShowing = false;

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
		if (MyController)
		{
			MyController->SetNameOfWidget(Pickup->ItemName);
			MyController->SetDescriptionOfWidget(Pickup->DescriptionText);
		}

	}//Re-Initialize 

	else 
		LastSeenItem = nullptr;
}

void ARobberCharacterClass::PickupItem()
{
	AObjectiveActor* objective = Cast<AObjectiveActor>(LastSeenItem);
	AObjectiveActor* objectivee = Cast<AObjectiveActor>(EquippedItem);


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
				SocketLocationR = Mesh1P->GetSocketLocation("GripPoint");
				EquippedItem->AttachRootComponentTo(Mesh1P, FName(TEXT("GripPoint")), EAttachLocation::SnapToTarget);
				EquippedItem->SetActorEnableCollision(false);
				currentInventoryIndex = 0;

				MyController->SetNameOfInventoryWidget(EquippedItem->ItemName);

				if (EquippedItem->ItemName == "Flashlight")
				{
					AFlashlightClass* aaaa = Cast<AFlashlightClass>(EquippedItem);
					if (aaaa)
					{
					//	aaaa->bIsActive = true;
					}
				}
			}

			else
			{
				FVector SocketLocationR;
				SocketLocationR = Mesh1P->GetSocketLocation("GripPoint");
				Inventory[AvailableSlots]->AttachRootComponentTo(Mesh1P, FName(TEXT("GripPoint")), EAttachLocation::SnapToTarget);
				Inventory[AvailableSlots]->SetActorEnableCollision(false);
				Inventory[AvailableSlots]->SetActorHiddenInGame(true);
			}
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
	FString a = "Flashlight";
	bHidden = false;
	//this bool is to ensure our binoculars auto zoom in the first time its switched to
	bManualZoom = false;
	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());
	//before we switch index, we switch our item to hidden in game so the next time can be shown
	Inventory[currentInventoryIndex]->SetActorHiddenInGame(true);
	
	/*Switching index to switch items*/
	if (currentInventoryIndex == 1)
	{
		if (Inventory[0] != nullptr)
			currentInventoryIndex++;
	}
	else if (currentInventoryIndex == 2)
	{
		if (Inventory[2] != nullptr)
			currentInventoryIndex = 0;
	}
	else if (Inventory[1] != nullptr)
		currentInventoryIndex++;

	/*if there is current index has an item*/
	if (Inventory[currentInventoryIndex])
	{
		//Sets Equipped Item to current inventory slot
		//Equipped item is so we can pull information like name and description of the item
		EquippedItem = Inventory[currentInventoryIndex];
		//Sets our UI to display name of equipped Item
		MyController->SetNameOfInventoryWidget(EquippedItem->ItemName);
		//Sets Equipped Item to show on screen
		EquippedItem->SetActorHiddenInGame(false);
	}

	/*if current item is flashlight*/
	if (Inventory[currentInventoryIndex]->ItemName == a)
	{
		AFlashlightClass* flashlight = Cast<AFlashlightClass>(EquippedItem);
		if (flashlight)
		{
			/*the bool on flashlight is set to true, this is so we know we can toggle the flash light*/
			/*there is a better fix but it will be implemented*/
			flashlight->bIsActive = true;
		}
	}

	if (Inventory[currentInventoryIndex]->ItemName == "Objective")
	{

		bHasObjective = true;
		
	}

	/*if its not the current index, it sets it to false*/
	/* i need to change the method because its searching all the current actors in the world right now*/
	else
	{
		for (TActorIterator<AFlashlightClass> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			AFlashlightClass *Mesh = *ActorItr;
			ActorItr->bIsActive = false;
		}
	}
	//Saftey check to ensure the black bincoular view is turned off if its already open
	if (MyController->bIsBinocularsOpen)
	{
		MyController->ToggleBinocularsWidgetOn();
	}

}

void ARobberCharacterClass::ToggleItemFunctions()
{
	/*casting to equipped item*/
	AFlashlightClass* flashlight = Cast<AFlashlightClass>(EquippedItem);
	ABinocularsClass* binoculars = Cast<ABinocularsClass>(EquippedItem);
	ALockpickClass* lockpick = Cast<ALockpickClass>(EquippedItem);
	ABoxClass* box = Cast<ABoxClass>(EquippedItem);
	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());

	//exception handling
	if (EquippedItem != nullptr)
	{
		//if current item is flashlight
		if (EquippedItem == flashlight)
		{
			if (flashlight)
			{
				//toggles it if its true to false and vice versa
				//bIsActive is called on blueprints to toggle the light
				if (flashlight->bIsActive == true)
				{
					flashlight->bIsActive = false;
				}
				else
					flashlight->bIsActive = true;
			}
		}
		//if current item is binoculars
		if (EquippedItem == binoculars)
		{
			//Turn on widget, the name of this funciton needs to be changed, it works as a toggle not just turning the widget on
			//the widget is the UI with the black screen and transparent holes
			MyController->ToggleBinocularsWidgetOn();
			if (MyController->bIsBinocularsOpen)
			{
				//sets the item as hidden if we are in the zoomed in view and the widget is active
				EquippedItem->SetActorHiddenInGame(true);
			}
			else
				//sets the item as shwon
				EquippedItem->SetActorHiddenInGame(false);
		}

		if (EquippedItem == box)
		{
			if (bHidden == false)
			{
				MyController->ToggleBoxWidgetOn();
				EquippedItem->SetActorHiddenInGame(true);
				bHidden = true;
			}
			else
			{
				MyController->ToggleBoxWidgetOff();
				EquippedItem->SetActorHiddenInGame(false);
				bHidden = false;
			}

		}
	}
}

void ARobberCharacterClass::BinocularsRaycast(float deltaseconds)
{
	//typical raycast
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * BinocularsRaycastRange);

	FHitResult RaycastHit;

	FCollisionQueryParams CPQ;
	CPQ.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CPQ);
	//DrawRay
	AEnemyAI* enemy = Cast<AEnemyAI>(RaycastHit.GetActor());

	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());

	if (LastSeenEnemy && LastSeenEnemy != enemy)
	{
		//If our character sees a different enemy then disable the glowing effect on the previous seen item
		LastSeenEnemy->SetTempOutline(false);
	}

	if (enemy)
	{
		EnemyTimer -= deltaseconds;
		LastSeenEnemy = enemy;
		enemy->SetTempOutline(true);
		if (EnemyTimer <= 0)
 		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::SanitizeFloat(EnemyTimer));
			enemy->bIsSet = true;
		}
	}
	else {
		LastSeenEnemy = nullptr;
		EnemyTimer = 1.5f;
	}
}


void ARobberCharacterClass::BinocularsZoomIn()
{
	float zoomAmount = -500.0f;
	bManualZoom = true;
	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());
	if (MyController)
	{
		if (MyController->bIsBinocularsOpen)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Zooming in"));
			if (CameraBoom->TargetArmLength <= 4000.0f)
			{
				CameraBoom->TargetArmLength = CameraBoom->TargetArmLength + zoomAmount;
			}
		}
	}
}

void ARobberCharacterClass::BinocularsZoomOut()
{
	float zoomAmount = 500.0f;
	bManualZoom = true;
	ARobberCharacterControllerClass* MyController = Cast<ARobberCharacterControllerClass>(GetController());
	if (MyController)
	{
		if (MyController->bIsBinocularsOpen)
		{
			// CameraBoom->TargetArmLength = (FMath::FInterpTo(CameraBoom->TargetArmLength, 0.0f, DeltaTime, 25.f));
			if (CameraBoom->TargetArmLength >= 1000.0f)
			{
				CameraBoom->TargetArmLength = CameraBoom->TargetArmLength - zoomAmount;
			}
		}
	}
}

void ARobberCharacterClass::Reset()
{
	ARobberCharacterControllerClass* pc = Cast<ARobberCharacterControllerClass>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	for (TActorIterator<ASpawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ASpawn *Mesh = *ActorItr;

		this->SetActorLocation(Mesh->GetActorLocation());
		bHasObjective = false;
		
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	if (pc)
	{
		pc->ToggleWastedWidgetOff();
		pc->ToggleWinWidgetOff();

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

		Inventory.Empty();

		if (EquippedItem != nullptr)
		{
			EquippedItem->Destroy(true);
		}
		EquippedItem = nullptr;

		
	}
}

void ARobberCharacterClass::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasObjective)
	{
		AHomeBaseClass* base = Cast<AHomeBaseClass>(OtherActor);
		if (OtherActor == base)
		{
			bHasObjective = false;
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

			ARobberCharacterControllerClass* pc = Cast<ARobberCharacterControllerClass>(GetController());
			if (pc)
			{
				pc->ToggleWinWidgetOn();

				FTimerHandle nothing;
				GetWorldTimerManager().SetTimer(nothing, this, &ARobberCharacterClass::Reset, 2.0f, false);
			}

		}

	}

}


