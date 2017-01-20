// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "AndroidCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AndroidController.h"


// Sets default values
AAndroidCharacter::AAndroidCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComponent->SetPeripheralVisionAngle(90.f);

}

// Called when the game starts or when spawned
void AAndroidCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAndroidCharacter::OnSeePlayer);
	}
}

void AAndroidCharacter::OnSeePlayer(APawn* Pawn)
{
	AAndroidController* AndroidController = Cast<AAndroidController>(GetController());

	if (AndroidController)
	{
		GLog->Log("I see you");
		AndroidController->SetSeenTarget(Pawn);
	}
}
