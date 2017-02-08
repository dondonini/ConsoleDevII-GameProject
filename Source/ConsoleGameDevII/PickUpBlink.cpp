// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleGameDevII.h"
#include "PickUpBlink.h"


// Sets default values
APickUpBlink::APickUpBlink()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CubeRoot"));
	RootComponent = CubeRoot;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->AttachToComponent(CubeRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CubeTriggerEnter = CreateDefaultSubobject<UBoxComponent>(TEXT("CubeTrigger"));
	CubeTriggerEnter->bGenerateOverlapEvents = true;
	CubeTriggerEnter->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	CubeTriggerEnter->OnComponentBeginOverlap.AddDynamic(this, &APickUpBlink::OnPlayerTriggerEnterBlink);
	CubeTriggerEnter->OnComponentEndOverlap.AddDynamic(this, &APickUpBlink::OnPlayerTriggerExitBlink);
	CubeTriggerEnter->AttachToComponent(CubeRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	BlinkAmount = 0.f;
}

// Called when the game starts or when spawned
void APickUpBlink::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpBlink::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,TEXT("Ticking"));

	if (bBlinkEffectTriggered)
	{
		if (BlinkAmount > 0.0f)
		{
			BlinkAmount -= DeltaTime;
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Triggering"));
		}
	}

	if (!bBlinkEffectTriggered)
	{
		if (BlinkAmount < 1.0f)
		{
			BlinkAmount += DeltaTime;
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Exiting"));
		}
	}
	UMaterialInstanceDynamic* BlinkMaterialInstance = CubeMesh->CreateDynamicMaterialInstance(0);

	if (BlinkMaterialInstance != nullptr)
	{
		BlinkMaterialInstance->SetScalarParameterValue(FName("BlinkAmount"), BlinkAmount);
	}
}

void APickUpBlink::OnPlayerTriggerEnterBlink(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bBlinkEffectTriggered = true;

}

void APickUpBlink::OnPlayerTriggerExitBlink(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bBlinkEffectTriggered = false;
}

