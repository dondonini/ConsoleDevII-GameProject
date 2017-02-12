// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasePickupClass.h"
#include "EnemyAI.h"
#include "RobberCharacterClass.generated.h"

#define MAX_INVENTORY_SLOTS 2
#define DEFAULT_MAX_SPEED 500

UCLASS()
class CONSOLEGAMEDEVII_API ARobberCharacterClass : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, Category = Camera)
	TSubclassOf<class UCameraShake> HeadBob;
	//class UCameraShake* FirstPersonCameraShake;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	void OnStartSprint();
	void OnStopSprint();

	void ToggleCrouch();

public:
	// Sets default values for this character's properties
	ARobberCharacterClass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void RaycastForward();

	void RaycastItem();

	ABasePickupClass* LastSeenItem;

	AEnemyAI* LastSeenEnemy;

	float EnemyTimer;
public:
	ABasePickupClass* EquippedItem;
private:

	UFUNCTION()
	void PickupItem();

	UPROPERTY(VisibleAnywhere)
	TArray<ABasePickupClass*> Inventory;

	UPROPERTY(EditAnywhere)
	FVector EquipmentVectorOffset;

	void NextItem();

	int32 currentInventoryIndex;

	void ToggleItemFunctions();

	void BinocularsRaycast(float deltaseconds);

	void BinocularsZoomIn();

	void BinocularsZoomOut();

	bool bManualZoom;

protected:
	UPROPERTY(EditDefaultsOnly)
	float RaycastRange;

	UPROPERTY(EditDefaultsOnly)
	float BinocularsRaycastRange;

	/* Player Movement */
	UPROPERTY(EditDefaultsOnly, Category = Player)
	float MaxSpeedDefault;

	UPROPERTY(EditDefaultsOnly, Category = Player)
	float SprintSpeedModifier;

	UPROPERTY(EditDefaultsOnly, Category = Player)
	float CrouchSpeedModifier;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FVector SpringArmOffset;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float SpringArmLength;

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	TArray<ABasePickupClass*> GetInventory() { return Inventory; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }




};
