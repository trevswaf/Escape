// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "EscapeGameModeBase.h"
#include "FPSCharacter.h"

AEscapeGameModeBase::AEscapeGameModeBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AFPSCharacter::StaticClass();
}




