// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Usable.generated.h"

UCLASS()
class ESCAPE_API AUsable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Overridable use function. Not called from this class, but called from Player activating it with input
	UFUNCTION(BlueprintNativeEvent, Category = Default)
	void Use();

	//Optional Mesh
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
};
