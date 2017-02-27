// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "FPSCharacter.h"
#include "AndroidCharacter.h"
#include "Usable.h"


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

	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	CharMovement->MaxWalkSpeed = 250;
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

	//Set our current usable to a result from this trace function, if we don't find one this value will be null.
	CurrentUsable = TraceForUsable();

	if (CurrentUsable)
	{
		OnFoundUsable.Broadcast(CurrentUsable->Prompt);
	}
	else
	{
		OnFoundUsable.Broadcast(TEXT(""));
	}

	//Tick Event that increments ChargeTime. Set true by ChargeShoot, and set false by Shoot.
	if (bIncrementCharge)
	{
		ChargeTime += DeltaTime;
	}

	OnChargeShot.Broadcast(ChargeTime);

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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::SetSprintSpeed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::SetWalkSpeed);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AFPSCharacter::UseUsable);
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
	if (bCanShoot && !bIsSprinting)
	{
		bIncrementCharge = true;
	}
	else
	{
		ChargeTime = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("cant charge"));
	}
}

void AFPSCharacter::Shoot()
{

	//If player cant shoot (is on cooldown) or is sprinting, they cant fire. Return.
	if (!bCanShoot || bIsSprinting || !bIncrementCharge)
	{
		ChargeTime = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("cant shoot"));
		return;
	}

	//flag variable to stop incrementing ChargeShoot in tick
	bIncrementCharge = false;

	//Comparator using the ChargeTime to determine what type of shot
	if (ChargeTime <= LowPowerShot)
	{
		ShotRange = 1000;
		ShotPower = 20;
		ShotCooldown = .15f; //.15
	}
	else if (ChargeTime <= HighPowerShot)
	{
		ShotRange = 1500;
		ShotPower = 50;
		ShotCooldown = .3f; //.3
	}
	else
	{
		ShotRange = 2000;
		ShotPower = 100;
		ShotCooldown = .5f; //.5
	}

	//Setting up trace variables
	FHitResult* Hit = new FHitResult();
	FVector StartTrace = FirstPersonCamera->GetComponentLocation();
	FVector DirectionVector = FirstPersonCamera->GetForwardVector();
	FVector EndTrace = (DirectionVector * ShotRange) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();

	//Do trace and check if we hit an android
	if (GetWorld()->LineTraceSingleByChannel(*Hit, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{

		AAndroidCharacter* Android = Cast<AAndroidCharacter>(Hit->GetActor());

		//if we did hit an android have them take damage
		if (Android)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Hit Android"));

			FDamageEvent DamageEvent;
			Android->TakeDamage(ShotPower, DamageEvent, GetController(), this);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,FString::SanitizeFloat(ChargeTime));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::FromInt(bCanShoot));//Prototyping held Shoot button TODO turn this into a set of arguments for shot range + power

	//Reset ChargeTime
	ChargeTime = 0.0f;

	//flag bCanShoot and register Timer to reset the flag based on ShotCooldown
	bCanShoot = false;
	OnShoot.Broadcast(false);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFPSCharacter::ResetShootCooldown, ShotCooldown, false);
}

void AFPSCharacter::ResetShootCooldown()
{
	bCanShoot = true;
	OnShoot.Broadcast(true);
}

void AFPSCharacter::SetWalkSpeed()
{
	UCharacterMovementComponent* tmp = GetCharacterMovement();
	tmp->MaxWalkSpeed = 250;
	bIsSprinting = false;
}

void AFPSCharacter::SetSprintSpeed()
{
	UCharacterMovementComponent* tmp = GetCharacterMovement();
	tmp->MaxWalkSpeed = 375;
	bIsSprinting = true;
}

AUsable* AFPSCharacter::TraceForUsable()
{
	//Store the trace result here
	FHitResult Hit;

	//Trace logistic variables, where we start, where we end
	FVector StartTrace = FirstPersonCamera->GetComponentLocation();
	FVector Direction = FirstPersonCamera->GetForwardVector();
	FVector EndTrace = StartTrace + (Direction * 150); /*Mgiht want to change this literal value to a variable*/

	//Set up the sphere shape we wil use to trace
	FCollisionShape TraceSphere;
	TraceSphere.ShapeType = ECollisionShape::Sphere;
	TraceSphere.SetSphere(25);

	//Set up the channel we are looking for. Usables have their own custom channel, refer to DefaultEngine.ini, but its ECC_GameTraceChannel1
	FCollisionObjectQueryParams COQP;
	COQP.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	FVector Center = ((EndTrace - StartTrace) / 2) + StartTrace;

	//DrawDebugSphere(GetWorld(), Center, TraceSphere.GetSphereRadius(), 100, FColor::Green);

	//Test our trace
	if (GetWorld()->SweepSingleByObjectType(Hit, StartTrace, EndTrace, FQuat::FQuat(), COQP,
		TraceSphere, FCollisionQueryParams::DefaultQueryParam))
	{
		//If you found a hit cast it to get it to the desired type
		return Cast<AUsable>(Hit.GetActor());
	}

	//If you are here then you are fucked. Luckily we test to make sure it isn't null in UseUable
	return nullptr;
}

void AFPSCharacter::UseUsable()
{
	if (CurrentUsable)
	{
		CurrentUsable->Use();
	}
}
