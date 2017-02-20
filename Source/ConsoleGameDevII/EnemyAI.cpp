// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "EnemyAIController.h"
#include "RobberCharacterClass.h"
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

	bIsSet = false;

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyAI::OnSeePlayer);
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
	if (AIController)
	{
		//Accesses the controller and casts the sensed pawn(player) into the enemy function of controller
		AIController->SetEnemy(SensedPawn);
	}
}

