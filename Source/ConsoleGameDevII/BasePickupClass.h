// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasePickupClass.generated.h"

UCLASS()
class CONSOLEGAMEDEVII_API ABasePickupClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickupClass();

	void SetOutline(bool Status);

	FORCEINLINE UTexture2D* GetPickUpTexture() { return PickupTexture; }
	
protected:
	/*The Static Mesh of the pickup*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PickupSM;

	/*The Texture of the item in case we want to add it in the secrets or inventory*/
	UPROPERTY(EditAnywhere, Category = "PickupProperties")
	UTexture2D* PickupTexture;

	/*The name of the item*/
	UPROPERTY(EditAnywhere, Category = "PickupProperties")
	FString ItemName;
};
