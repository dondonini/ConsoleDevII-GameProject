// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "EnemyAIController.h"
#include "RobberCharacterClass.h"
#include "RobberCharacterControllerClass.h"
#include "ConsoleGameState.h"
#include "Spawn.h"
/* AI Include */
#include "Perception/PawnSensingComponent.h"
#include "EnemyAI.h"




// Sets default values
AEnemyAI::AEnemyAI(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();

	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->AttachTo(RootComponent);

	DefaultMaxSpeed = 300.f;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->AttachTo(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->AttachTo(RootComponent);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComp->SetPeripheralVisionAngle(45.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	BotType = EBotBehaviorType::Patrolling;

	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

	SprintSpeedModifier = 2.5f;

	SenseTimeOut = 2.5f;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::OnBoxOverlap);


	bIsSet = false;

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyAI::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyAI::OnHearNoise);
	}
	
}

// Called every frame
void AEnemyAI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsSet == true)
		SetOutline(true);

	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut && (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
	{
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		if (AIController)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Sensed Target:" + bSensedTarget);
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);

		}
	}
	if (bSensedTarget)
	{
		GetCharacterMovement()->MaxWalkSpeed = (DefaultMaxSpeed * SprintSpeedModifier);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
	}

	if (bSlowDown)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2);
	}
}

// Called to bind functionality to input


void AEnemyAI::SetTempOutline(bool Status)
{
	EnemyMesh->SetRenderCustomDepth(Status);
}

void AEnemyAI::SetOutline(bool Status)
{
	EnemyMesh->SetRenderCustomDepth(Status);
}

void AEnemyAI::SetBotType(EBotBehaviorType NewType)
{
	BotType = NewType;

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->SetBlackboardBotType(NewType);
	}
}

void AEnemyAI::OnSeePlayer(APawn* Pawn)
{
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	ARobberCharacterClass* SensedPawn = Cast<ARobberCharacterClass>(Pawn);
	if (SensedPawn->bHidden == false)
	{
		if (AIController)
		{
			//Accesses the controller and casts the sensed pawn(player) into the enemy function of controller
			AIController->SetEnemy(SensedPawn);
		}
	}
}

void AEnemyAI::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	bSensedTarget = true;
	LastHeardTime = GetWorld()->GetTimeSeconds();

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->SetEnemy(PawnInstigator);
	}
}
void AEnemyAI::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UGameplayStatics::SetGamePaused(GetWorld(), true);

	ARobberCharacterControllerClass* pc = Cast<ARobberCharacterControllerClass>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc)
	{
		if (bSensedTarget)
		{
			bSlowDown = true;
			pc->ToggleWastedWidgetOn();
			FTimerHandle nothing;
			GetWorldTimerManager().SetTimer(nothing, this, &AEnemyAI::ToggleTimer, 2.0f, false);
			/* run a timer here and reset everything*/
		}
	}

}

void AEnemyAI::ToggleTimer()
{
	ARobberCharacterControllerClass* pc = Cast<ARobberCharacterControllerClass>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc)
	{
		ARobberCharacterClass* character = Cast<ARobberCharacterClass>(pc->GetPawn());
		if (character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("accessed character"));
			character->Reset();		
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RSpawning"));
		bSlowDown = false;

		ARobberCharacterControllerClass* pc = Cast<ARobberCharacterControllerClass>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pc)
			pc->ToggleWastedWidgetOff();


	}


}



