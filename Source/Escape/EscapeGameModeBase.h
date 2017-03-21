// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "EscapeGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EQuestStage : uint8
{
	Intro,
	GrabGun,
	DoorLocked,
	AndroidEncounter,
	GrabKey,
	TurnOnPower,
	PowerTurnedOn,
	TransferData,
	DataTransferred
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestStageDelegate, EQuestStage, Stage);

UCLASS()
class ESCAPE_API AEscapeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AEscapeGameModeBase(const FObjectInitializer& ObjectIntializer);

public:

	UFUNCTION(BlueprintCallable, Category = Quest)
	void SetQuestStage(EQuestStage Stage);

	UFUNCTION(BlueprintCallable, Category = Quest)
	EQuestStage GetQuestStage();

	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestStageDelegate OnQuestStageSet;
	
private:
	EQuestStage QuestStage;
};
