// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "NonPlayerCharacter.h"


// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->nMaxHealth = 100;
	this->nCurHealth = nMaxHealth;
}

// Called when the game starts or when spawned
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANonPlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ANonPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{ 
	Super::SetupPlayerInputComponent(InputComponent);

}
