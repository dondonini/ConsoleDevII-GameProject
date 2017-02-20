// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAI.h"
#include "Waypoints.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BotTypeKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;
	/*Blackboard keys*/
public:
	AEnemyAIController();

	virtual void Possess(APawn* Pawn) override;

	virtual void UnPossess() override;

	UFUNCTION(BlueprintCallable, Category = AI)
	void SearchForEnemy();

	void SetWaypoint(AWaypoints* NewWaypoint);

	void SetEnemy(class APawn* Pawn);

	AWaypoints* GetWaypoint();

	void SetBlackboardBotType(EBotBehaviorType NewType);


	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorTreeComponent; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComponent; }

protected:
	UPROPERTY(VisibleAnywhere)
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackboardComponent;

protected:
	uint8 EnemyKeyID;
	uint8 EnemyLocationID;
	
	
	
	
};
