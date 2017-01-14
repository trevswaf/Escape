// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "FPSCharacter.h"


// Sets default values
AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FirstPersonCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonCamera->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
	FirstPersonCamera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Tick Event that increments ChargeTime. Set true by ChargeShoot, and set false by Shoot.
	if (bIncrementCharge)
	{
		ChargeTime += DeltaTime;
	}

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnEndJump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AFPSCharacter::ChargeShoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AFPSCharacter::Shoot);
}

//Called by player input to add forward motion to character
void AFPSCharacter::MoveForward(float val)
{
	if ((Controller != NULL) && (val != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();

		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}

		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, val);
	}
}

void AFPSCharacter::MoveRight(float val)
{
	if ((Controller != NULL) && (val != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		AddMovementInput(Direction, val);
	}
}

void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::OnEndJump()
{
	bPressedJump = false;
}

void AFPSCharacter::ChargeShoot()
{
	bIncrementCharge = true;
}

void AFPSCharacter::Shoot()
{

	//flag variable to stop incrementing ChargeShoot in tick
	bIncrementCharge = false;

	//Comparator using the ChargeTime to determine what type of shot
	if (ChargeTime <= LowPowerShot)
	{
		ShotRange = 1000;
		ShotPower = 20;
		ShotCooldown = .15f;
	}
	else if (ChargeTime <= HighPowerShot)
	{
		ShotRange = 1500;
		ShotPower = 50;
		ShotCooldown = 0.3f;
	}
	else
	{
		ShotRange = 2000;
		ShotPower = 100;
		ShotCooldown = 0.5f;
	}

	//Setting up trace variables
	FHitResult* Hit = new FHitResult();
	FVector StartTrace = FirstPersonCamera->GetComponentLocation();
	FVector DirectionVector = FirstPersonCamera->GetForwardVector();
	FVector EndTrace = (DirectionVector * ShotRange) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();

	//Do trace and draw debug if successful hit returned
	if (GetWorld()->LineTraceSingleByChannel(*Hit, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, true);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,FString::SanitizeFloat(ChargeTime)); //Prototyping held Shoot button TODO turn this into a set of arguments for shot range + power

	//Reset ChargeTime
	ChargeTime = 0.0f;

	//TODO: flag bCanShoot and register Timer to reset the flag based on ShotCooldown
}