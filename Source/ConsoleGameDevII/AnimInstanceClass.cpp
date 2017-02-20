// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "EnemyAI.h"
#include "AnimInstanceClass.h"


void UAnimInstanceClass::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	AEnemyAI* AICharacter = Cast<AEnemyAI>(TryGetPawnOwner());
	if (AICharacter)
	{
		AISpeed = AICharacter->GetVelocity().Size();
	}
}


