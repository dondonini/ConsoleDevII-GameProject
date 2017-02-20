// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "EnemyAIController.h"
#include "RobberCharacterClass.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	PatrolLocationKeyName = "PatrolLocation";
	CurrentWaypointKeyName = "CurrentWaypoint";
	BotTypeKeyName = "BotType";
	TargetEnemyKeyName = "TargetEnemy";
}

void AEnemyAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AEnemyAI* AICharacter = Cast<AEnemyAI>(Pawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*AICharacter->BehaviorTree->BlackboardAsset);

			SetBlackboardBotType(AICharacter->BotType);

// 			EnemyKeyID = BlackboardComponent->GetKeyID("Enemy");
// 			EnemyLocationID = BlackboardComponent->GetKeyID("Destination");
		}

		BehaviorTreeComponent->StartTree(*AICharacter->BehaviorTree);

	}
}

void AEnemyAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorTreeComponent->StopTree();
}

void AEnemyAIController::SearchForEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	ARobberCharacterClass *BestPawn = NULL;

	for (FConstPawnIterator ActorItr = GetWorld()->GetPawnIterator(); ActorItr; ++ActorItr)
	{
		ARobberCharacterClass* PlayerCharacter = Cast<ARobberCharacterClass>(*ActorItr);
		if (PlayerCharacter)
		{
			const float DistSq = FVector::Dist(PlayerCharacter->GetActorLocation(), MyLoc);
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				if (BestPawn->bHidden == false)
					BestPawn = PlayerCharacter;
				else
					BestPawn = nullptr;
			}
		}
	}
	if (BestPawn)
	{
		if (BestPawn->bHidden == false)
			SetEnemy(BestPawn);
	}
}

void AEnemyAIController::SetEnemy(class APawn* Pawn)
{
	ARobberCharacterClass* character = Cast<ARobberCharacterClass>(Pawn);

	if (BlackboardComponent)
		BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, nullptr);

	if (BlackboardComponent)
	{
		if (character)
		{
			if (character->bHidden == false)
				BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, Pawn);
			else
				BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, nullptr);
		}
	}
}

void AEnemyAIController::SetBlackboardBotType(EBotBehaviorType NewType)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsEnum(BotTypeKeyName, (uint8)NewType);
	}
}

void AEnemyAIController::SetWaypoint(AWaypoints* NewWaypoint)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(CurrentWaypointKeyName, NewWaypoint);
	}
}

AWaypoints* AEnemyAIController::GetWaypoint()
{
	if (BlackboardComponent)
	{
		return Cast<AWaypoints>(BlackboardComponent->GetValueAsObject(CurrentWaypointKeyName));
	}

	return nullptr;
}

