// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "FPSPlayerController.h"
#include "EscapeGameModeBase.h"


//this will work to diagnose if we're using gamepad or not. Solution for in game UI prompts. Can set a variable here or HUD, that widget can grab?
bool AFPSPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (!bGamepad)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("not using gamepad"));
	}

	AEscapeGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AEscapeGameModeBase>();

	if (GameMode)
	{
		GameMode->bIsUsingGamepad = bGamepad;
	}

	return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
}
