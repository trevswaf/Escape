// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AndroidCharacter.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "AI")
	AActor* DefaultLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	//time that clears player was sensed. MUST BE LARGER THAN SENSE INTERVAL
	UPROPERTY (EditDefaultsOnly, Category = "AI")
	float SeenThreshhold = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float Health = 100;

private:

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	//used to track when player was last sensed
	float LastSeenTime;
	
	bool bSeenPlayer = false;

};
