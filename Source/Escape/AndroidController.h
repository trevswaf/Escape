// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AndroidController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AAndroidController : public AAIController
{
	GENERATED_BODY()
	
public:

	AAndroidController();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardKey = "Target";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationKey = "DefaultLocation";

	virtual void Possess(APawn* Pawn) override;

	void SetSeenTarget(APawn* Pawn);

	void SetDefaultLocation(FVector Location);


private:

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;
	
};
