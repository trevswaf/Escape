// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Usable.h"


// Sets default values
AUsable::AUsable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AUsable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AUsable::Use_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("you used me D:"));
}

