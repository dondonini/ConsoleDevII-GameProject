// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "AnimInstanceClass.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEGAMEDEVII_API UAnimInstanceClass : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AISpeed;
	
	
};
