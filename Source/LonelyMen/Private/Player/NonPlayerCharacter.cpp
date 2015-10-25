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

float ANonPlayerCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage;
	ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		nCurHealth -= ActualDamage;
		if (nCurHealth <= 0)
		{
			PlayDie(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}

	}
	
	return ActualDamage;
}

void ANonPlayerCharacter::PlayDie(float KillingDamge, FDamageEvent const& DamageEvent, AController* killer, AActor* DamageCauser)
{
	nCurHealth = FMath::Min(0, nCurHealth);

	if (bIsDying)
		return;

	//play death sound
	
	//remove 
	DestroyAllInventory();
	DetachFromControllerPendingDestroy();
	
	//disable collison klon capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	if (GetMesh())
	{
		static FName CollisonProfileName(TEXT("Radoll"));
		GetMesh()->SetCollisionProfileName(CollisonProfileName);
	}

	SetActorEnableCollision(true);

	//play death anim
	float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	//Ragdoll
	if (DeathAnimDuration > 0)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ANonPlayerCharacter::SetRagDollPhysics, FMath::Min(0.1f, DeathAnimDuration), false);
	}
	else
	{
		SetRagDollPhysics();
	}
}

void ANonPlayerCharacter::PlayHit(float DamageTaken, struct FDamageEvent const & DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	USkeletalMeshComponent* UseMesh = GetMesh();

	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		float HitAnimDuration = UseMesh->AnimScriptInstance->Montage_Play(HitAnim);
		
		GetCharacterMovement()->Velocity = FVector::ZeroVector;

	}
}

void ANonPlayerCharacter::DestroyAllInventory()
{

}

void ANonPlayerCharacter::StopAllAnimMontage()
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}

float ANonPlayerCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate /* = 1.f */, FName StartSectionName /* = NAME_None */)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}
	return 0.0f;
}

void ANonPlayerCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent *UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance && UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOutTime);
	}
}

void ANonPlayerCharacter::SetRagDollPhysics()
{
	bool bInRagdoll = false;

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		//init physics
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
		
		bInRagdoll = true;
	}


	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		//hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		float beginTime, endTime;

		GetMesh()->AnimScriptInstance->GetCurrentActiveMontage()->GetSectionStartAndEndTime(0, beginTime, endTime);

		SetLifeSpan(endTime - endTime*0.4);
	}


}
