// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "AndroidController.h"
#include "AndroidCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAndroidController::AAndroidController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AAndroidController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AAndroidCharacter* Android = Cast<AAndroidCharacter>(Pawn);
	if (Android)
	{
		if (Android->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(Android->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*Android->BehaviorTree);
		}
	}
}

void AAndroidController::SetSeenTarget(APawn* Pawn)
{
	if (BlackboardComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,(TEXT("%s"), Pawn->GetName()));
		BlackboardComp->SetValueAsObject(BlackboardKey, Pawn);
	}
}
