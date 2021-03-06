// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "AndroidCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AndroidController.h"
#include "FPSCharacter.h"


// Sets default values
AAndroidCharacter::AAndroidCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up Pawn Sensing
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComponent->SetPeripheralVisionAngle(90.f);

	//Randomize the run speed between 200 and 500 in increments of 50.
	RunSpeed = FMath::RandRange(4, 10) * 50.f;
}

// Called when the game starts or when spawned
void AAndroidCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAndroidCharacter::OnSeePlayer);
	}

	AAndroidController* AndroidController = Cast<AAndroidController>(GetController());

	if (AndroidController && DefaultLocation)
	{
		AndroidController->SetDefaultLocation(DefaultLocation->GetActorLocation());
	}
}

void AAndroidCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ((GetWorld()->GetTimeSeconds() - SeenThreshhold) > LastSeenTime && bSeenPlayer)
	{
		AAndroidController* Controller = Cast<AAndroidController>(GetController());
		if (Controller)
		{
			UCharacterMovementComponent* tmp = GetCharacterMovement();
			tmp->MaxWalkSpeed = WalkSpeed;
			bSeenPlayer = false;
			GLog->Log("Sight Lost");
			Controller->SetSeenTarget(nullptr);
		}
	}
}

float AAndroidCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		OnDeath.Broadcast();
	}

	OnTakeDamage.Broadcast();

	return DamageAmount;
}

void AAndroidCharacter::InitDefaultLocation()
{
	AAndroidController* AndroidController = Cast<AAndroidController>(GetController());

	if (AndroidController && DefaultLocation)
	{
		AndroidController->SetDefaultLocation(DefaultLocation->GetActorLocation());
	}
}

void AAndroidCharacter::OnSeePlayer(APawn* Pawn)
{
	AAndroidController* AndroidController = Cast<AAndroidController>(GetController());

	if (AndroidController)
	{
		UCharacterMovementComponent* tmp = GetCharacterMovement();
		tmp->MaxWalkSpeed = RunSpeed;
		GLog->Log("I see you");
		bSeenPlayer = true;
		LastSeenTime = GetWorld()->GetTimeSeconds();
		AndroidController->SetSeenTarget(Pawn);
	}
}

void AAndroidCharacter::OnDamageOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);
		if (Player != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("hit player"));
			FDamageEvent DamageEvent;

			Player->TakeDamage(MeleeDamage, DamageEvent, GetController(), this);

			bCanDamage = false;
		}
	}
}
