// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AndroidCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeDamageDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS()
class ESCAPE_API AAndroidCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAndroidCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	AActor* DefaultLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 500;

	//time that clears player was sensed. MUST BE LARGER THAN SENSE INTERVAL
	UPROPERTY (EditDefaultsOnly, Category = "AI")
	float SeenThreshhold = 100.f;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float Health = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float MeleeDamage = 12.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	bool bCanDamage = true;

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FTakeDamageDelegate OnTakeDamage;

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FDeathDelegate OnDeath;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void InitDefaultLocation();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void OnDamageOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	//used to track when player was last sensed
	float LastSeenTime;
	
	bool bSeenPlayer = false;

};
