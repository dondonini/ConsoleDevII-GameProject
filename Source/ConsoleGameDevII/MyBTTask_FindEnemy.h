// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_FindEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API UMyBTTask_FindEnemy : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
