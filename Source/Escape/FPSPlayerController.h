// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual bool InputKey(FKey Key,EInputEvent EventType,float AmountDepressed,bool bGamepad) override;

	//TODO add widget here. Create widget variable
	
	
};
