// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LonelyMenHero.h"


ALonelyMenHero::ALonelyMenHero()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->SetRelativeRotation(FRotator(300.0f, 0.0f, 0.0f));
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	this->CameraZoomMin = 400;
	this->CameraZoomMax = 1200;
	this->CameraZoomRate = 50;
	this->GoalArmLength = CameraBoom->TargetArmLength;

}
//////////////////////////////////////////////////////////////////////////
// Input
void ALonelyMenHero::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ALonelyMenHero::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALonelyMenHero::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &ALonelyMenCharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &ALonelyMenCharacter::LookUpAtRate);

	//绑定相机滚轮
	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ALonelyMenHero::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ALonelyMenHero::CameraZoomOut);

	//绑定攻击
	//InputComponent->BindAction("Fire", IE_Pressed, this, &ALonelyMenCharacter::OnStartFire);
	//InputComponent->BindAction("Fire", IE_Released, this, &ALonelyMenCharacter::OnStopFire);
}

void ALonelyMenHero::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALonelyMenHero::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALonelyMenHero::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALonelyMenHero::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALonelyMenHero::CameraZoomIn()
{
	this->GoalArmLength -= this->CameraZoomRate;
}

void ALonelyMenHero::CameraZoomOut()
{
	this->GoalArmLength += this->CameraZoomRate;
}

void ALonelyMenHero::AdjuestCameraArmLength(float DeltaTime)
{
	if (this->GoalArmLength != CameraBoom->TargetArmLength)
	{

		float tmpLength = FMath::FInterpTo(CameraBoom->TargetArmLength, this->GoalArmLength, DeltaTime, this->CameraZoomRate / 10);
		CameraBoom->TargetArmLength = FMath::Clamp<float>(tmpLength, this->CameraZoomMin, this->CameraZoomMax);
	}
}

void ALonelyMenHero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	this->AdjuestCameraArmLength(DeltaSeconds);

}

