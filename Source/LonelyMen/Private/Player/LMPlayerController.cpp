// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMPlayerController.h"


ALMPlayerController::ALMPlayerController()
{
	this->bShowMouseCursor = true;
}

void ALMPlayerController :: RotationChange()
{
	this->bIsRotationChange = true;
}

void ALMPlayerController::StopRotationChange()
{
	this->bIsRotationChange = false;
}

void ALMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("ChangeRotation", IE_Pressed, this, &ALMPlayerController::RotationChange);
	InputComponent->BindAction("StopChangeRotation", IE_Released, this, &ALMPlayerController::StopRotationChange);
}

void ALMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}