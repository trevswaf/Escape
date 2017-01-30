// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "EscapeGameModeBase.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"

AEscapeGameModeBase::AEscapeGameModeBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AFPSCharacter::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();
}

void AEscapeGameModeBase::SetQuestStage(EQuestStage Stage)
{
	QuestStage = Stage;
	OnQuestStageSet.Broadcast(Stage);
}




