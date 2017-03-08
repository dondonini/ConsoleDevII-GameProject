// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAI.generated.h"

UENUM()
enum class EBotBehaviorType : uint8
{
	Passive,
	Patrolling,
};

UCLASS(ABSTRACT)
class CONSOLEGAMEDEVII_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/* Change default bot type during gameplay */
	void SetBotType(EBotBehaviorType NewType);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	EBotBehaviorType BotType;

	/*The Static Mesh of the pickup*/
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* EnemyMesh;

	void SetTempOutline(bool Status);

	void SetOutline(bool Status);

public:
	bool bIsSet;

protected:
	/*If the player is inside this box component he will be able to initiate a conversation with the pawn*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	/*The audio component responsible for playing the dialog coming from this pawn*/
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UPawnSensingComponent* PawnSensingComp;

	bool bSensedTarget;

	float SenseTimeOut;

	float LastSeenTime;

	float LastHeardTime;

	float SprintSpeedModifier;

	float DefaultMaxSpeed;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	bool bSlowDown;

	void ToggleTimer();
private:
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	
	
};
