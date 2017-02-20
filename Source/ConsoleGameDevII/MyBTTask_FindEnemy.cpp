// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "RobberCharacterClass.h"
#include "MyBTTask_FindEnemy.h"



//iterates through the world looking for actor (ATrappedByWarcharacter) when it finds it, it selects it as the enemy
EBTNodeResult::Type UMyBTTask_FindEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!GetWorld()) // if there is no world, return fail
	{
		return EBTNodeResult::Failed;
	}

	for (TActorIterator<ARobberCharacterClass> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARobberCharacterClass* PlayerCharacter = *ActorItr;
		if (PlayerCharacter)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), PlayerCharacter);

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
