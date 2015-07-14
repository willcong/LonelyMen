// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "LonelyMenCharacter.generated.h"

UCLASS(config=Game)
class ALonelyMenCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ALonelyMenCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	/** 相机最小拉近距离 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomMin;

	/** 相机最大拉远距离 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomMax;

	/** 相机缩放系数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomRate;
	/** 相机与角色新距离 */
	float GoalArmLength;

protected:
	/**相机拉近*/
	//UFUNCTION(BlueprintCallable,Category="Camera")
	void CameraZoomIn();
	/**相机拉远*/
	//UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraZoomOut();

	void AdjuestCameraArmLength(float DeltaTime);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	// End of APawn interface
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

