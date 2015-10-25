// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "NonPlayerCharacter.generated.h"

UCLASS()
class LONELYMEN_API ANonPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANonPlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/* when Character take damage but not die */
	void PlayHit(float DamageTaken,struct FDamageEvent const & DamageEvent,class APawn* PawnInstigator,class AActor* DamageCauser);
	
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;
	
	/* when Character take damage and is die */
	void PlayDie(float KillingDamge,FDamageEvent const& DamageEvent,AController* killer,AActor* DamageCauser);

	void DestroyAllInventory();

	void StopAllAnimMontage();

	void SetRagDollPhysics();

	/************************************************************************/
	/* 
		Character Porterty
	*/
	/************************************************************************/
protected:
	int nCurHealth;
	int nMaxHealth;

	/* check is Dying */
	uint32 bIsDying : 1;

	/* die anim */
	UPROPERTY(EditDefaultsOnly,Category=Animation)
	UAnimMontage *DeathAnim;

	/* hit anim */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage *HitAnim;
};
