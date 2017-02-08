// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickUpBlink.generated.h"

UCLASS()
class CONSOLEGAMEDEVII_API APickUpBlink : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBlink();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Dummy root component
	UPROPERTY(EditAnywhere)
		USceneComponent* CubeRoot;
	
	//the mesh for cube
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CubeMesh;
	//the trigger Enter for cube
	UPROPERTY(EditAnywhere)
		UBoxComponent* CubeTriggerEnter;

	//need to switch to distance not overlapp with other actor
	UFUNCTION()
		void OnPlayerTriggerEnterBlink(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnPlayerTriggerExitBlink(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	bool bBlinkEffectTriggered;

	float BlinkAmount;
};
