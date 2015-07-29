// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LonelyMen.h"
#include "LonelyMenCharacter.h"
#include "Weapon/LMWeapon.h"

//////////////////////////////////////////////////////////////////////////
// ALonelyMenCharacter

ALonelyMenCharacter::ALonelyMenCharacter()
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
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	bWantsToFire = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALonelyMenCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ALonelyMenCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALonelyMenCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &ALonelyMenCharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &ALonelyMenCharacter::LookUpAtRate);

	//绑定相机滚轮
	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ALonelyMenCharacter::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ALonelyMenCharacter::CameraZoomOut);

	//绑定攻击
	InputComponent->BindAction("Fire", IE_Pressed, this, &ALonelyMenCharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ALonelyMenCharacter::OnStopFire);
}

void ALonelyMenCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALonelyMenCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALonelyMenCharacter::MoveForward(float Value)
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

void ALonelyMenCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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

void ALonelyMenCharacter::CameraZoomIn()
{
	this->GoalArmLength -= this->CameraZoomRate;
}

void ALonelyMenCharacter :: CameraZoomOut()
{
	this->GoalArmLength += this->CameraZoomRate;
}

void ALonelyMenCharacter::AdjuestCameraArmLength(float DeltaTime)
{
	if (this->GoalArmLength != CameraBoom->TargetArmLength)
	{
		
		float tmpLength = FMath::FInterpTo(CameraBoom->TargetArmLength, this->GoalArmLength, DeltaTime, this->CameraZoomRate/10);
		CameraBoom->TargetArmLength = FMath::Clamp<float>(tmpLength, this->CameraZoomMin, this->CameraZoomMax);
	}
}

void ALonelyMenCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	this->AdjuestCameraArmLength(DeltaSeconds);

}

void ALonelyMenCharacter::OnStartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		if (CurrentWeapon != nullptr)
		{
			CurrentWeapon->StartFire();
		}
	}
}

void ALonelyMenCharacter::OnStopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		if (CurrentWeapon != nullptr)
		{
			CurrentWeapon->StopFire();
		}
	}
}

bool ALonelyMenCharacter::IsFiring() const
{
	return bWantsToFire;
}

void ALonelyMenCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnDefaultInventory();
}

void ALonelyMenCharacter::SpawnDefaultInventory()
{
	int32 InventoryCount = this->DefaultInventoryClasses.Num();
	for (int32 i = 0; i < InventoryCount;i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.bNoCollisionFail = true;
			ALMWeapon* NewWeapon = GetWorld()->SpawnActor<ALMWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}

	if (Inventory.Num()>0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void ALonelyMenCharacter::AddWeapon(ALMWeapon* weapon)
{
	if (weapon)
	{
		weapon->OnEnterInventory(this);
		Inventory.Add(weapon);
	}
}

void ALonelyMenCharacter::RemoveWeapon(ALMWeapon* weapon)
{
	if (weapon)
	{
		weapon->OnLeaveInventory();
		Inventory.RemoveSingle(weapon);
	}
}

void ALonelyMenCharacter::EquipWeapon(class ALMWeapon* weapon)
{
	if (weapon)
	{
		SetCurrentWeapon(weapon);
	}
}

void ALonelyMenCharacter::SetCurrentWeapon(class ALMWeapon* NewWeapon, class ALMWeapon* LastWeapon /* = NULL */)
{
	ALMWeapon* LocalLastWeapon = NULL;

	if (LastWeapon!= NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon != NULL)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	if (CurrentWeapon != NULL)
	{
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
	}
}

FName ALonelyMenCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}