// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "LonelyMenCharacter.generated.h"

UCLASS(Abstract,config = Game)
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

	//////////////////////////////////////////////////////////////////////////
	// ��������
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	/** �����С�������� */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomMin;

	/** ��������Զ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomMax;

	/** �������ϵ�� */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomRate;
	/** ������ɫ�¾��� */
	float GoalArmLength;

protected:
	/**�������*/
	//UFUNCTION(BlueprintCallable,Category="Camera")
	void CameraZoomIn();
	/**�����Զ*/
	//UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraZoomOut();

	void AdjuestCameraArmLength(float DeltaTime);
	//////////////////////////////////////////////////////////////////////////
	// �������
protected:
	/** currenly equipped weapon */
	UPROPERTY(Transient)
	class ALMWeapon* CurrentWeapon;
	
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditDefaultsOnly ,Category=Inventory)
	FName WeaponAttachPoint;

	/** current firing state */
	uint8 bWantsToFire : 1;

	/** which sub class of ALMWeapon can use ɸѡ��ALMWeapon������ */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<class ALMWeapon>> DefaultInventoryClasses;

	UPROPERTY(Transient)
	TArray<class ALMWeapon*> Inventory;

	/** get firing state */
	UFUNCTION(BlueprintCallable,Category="Game|Weapon")
	bool IsFiring() const;

	/** ����Ĭ�ϵ����� */
	void SpawnDefaultInventory();

	/** ��������ʼ���������������б� */
	void AddWeapon(class ALMWeapon* weapon);

	/** ���������������б����Ƴ� */
	void RemoveWeapon(class ALMWeapon* weapon);

	/** װ����������ɫ���� */
	void EquipWeapon(class ALMWeapon* weapon);

	/**  ����������Ϊ��ǰ���� */
	void SetCurrentWeapon(class ALMWeapon* NewWeapon, class ALMWeapon* LastWeapon = NULL);
public:
	/** get weapon attach point */
	FName GetWeaponAttachPoint() const;
protected:
	//////////////////////////////////////////////////////////////////////////
	// ����

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

	/** player pressed start fire action */
	void OnStartFire();

	/** player released start fire action */
	void OnStopFire();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	/** spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;
	// End of APawn interface
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

