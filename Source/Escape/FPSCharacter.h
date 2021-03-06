// Fill out your copyright notice in the Description page of Project Settings.
//Trevor Swafford

#pragma once

#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUseDelegate, FString, Prompt, bool, bUsable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeDelegate, float, ChargeTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShootDelegate, bool, bCanShoot, float, Cooldown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageDelegate, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintDelegate, bool, bIsSprinting);

UCLASS()
class ESCAPE_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FirstPersonCamera;

	//threshhold for a low power shot
	UPROPERTY(EditAnywhere, Category = Shooting)
	float LowPowerShot = 0.2f;

	// So this means medium power shot lies between .2 and 1 seconds held.

	//threshhold for a high power shot
	UPROPERTY(EditAnywhere, Category = Shooting)
	float HighPowerShot = 1.0f;

	UPROPERTY()
	AUsable* CurrentUsable;

	UPROPERTY(BlueprintAssignable, Category = Use)
	FUseDelegate OnFoundUsable;

	UPROPERTY(BlueprintAssignable, Category = Shooting)
	FChargeDelegate OnChargeShot;

	UPROPERTY(BlueprintAssignable, Category = Shooting)
	FShootDelegate OnShoot;

	UPROPERTY(BlueprintAssignable, Category = Default)
	FDamageDelegate OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = Default)
	FSprintDelegate OnSprint;

	UPROPERTY(BlueprintReadWrite, Category = Default)
	float Health = 100.f;

	// Sets default values for this character's properties
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

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

	UFUNCTION(BlueprintCallable, Category = Shooting)
	void ResetShootCooldown();

	UFUNCTION()
	void SetWalkSpeed();

	UFUNCTION()
	void SetSprintSpeed();

	//Called in Tick to sphere cast for a usable in front of us
	UFUNCTION()
	AUsable* TraceForUsable();

	//If we have a current usable set, call it's use function
	UFUNCTION()
	void UseUsable();

protected:

	//flag that determines if player can shoot or if gun is on cooldown -- Initially set to false bc player doesnt start with weapon.
	//we call reset function on pickup gun.
	UPROPERTY(BlueprintReadOnly, Category = "Shooting")
	bool bCanShoot = false;

private:

	//amount of time shoot button has been held. used to get parameters for shot range/strength
	float ChargeTime = 0.0f;

	//flag variable that controls whether to increment charge in tick. set true in ChargeShoot (on shoot pressed) , set false in Shoot (on shoot released)
	bool bIncrementCharge = false;

	//range of shot - determined by ChargeTime
	float ShotRange;

	//power of shot - determined by ChargeTime
	float ShotPower;

	//power of shot - determined by type of shot fired
	float ShotCooldown;

	//flag that determines if player is sprinting
	bool bIsSprinting = false;

};
