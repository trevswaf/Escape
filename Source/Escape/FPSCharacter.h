// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class ESCAPE_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FirstPersonCamera;

	// Sets default values for this character's properties
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Called to give player forward/backward movement. Bound in SetupPlayerInputComponent
	UFUNCTION()
	void MoveForward(float val);

	//Called to give player left/right movement. Bound in SetupPlayerInoutComponent
	UFUNCTION()
	void MoveRight(float val);

	//Called to make player jump. Bound in SetupPlayerInputComponent
	UFUNCTION()
	void OnStartJump();

	//Called to make player end jump. Bound in SetupPlayerInputComponent
	UFUNCTION()
	void OnEndJump();

	//Called to flag bIncrementCharge in tick, increments ChargeTime, which will be passed into Shoot to determine shot parameters
	UFUNCTION()
	void ChargeShoot();

	//Called to unflag bIncrementCharge, and take the results of ChargeTime, process them, and trace with those parameters to check for hit
	UFUNCTION()
	void Shoot();

private:

	//amount of time shoot button has been held. used to get parameters for shot range/strength
	float ChargeTime = 0.0f;

	//flag variable that controls whether to increment charge in tick. set true in ChargeShoot (on shoot pressed) , set false in Shoot (on shoot released)
	bool bIncrementCharge = false;
};
