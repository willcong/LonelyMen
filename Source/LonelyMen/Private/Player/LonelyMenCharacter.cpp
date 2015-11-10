// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LonelyMen.h"
#include "LonelyMenCharacter.h"
#include "Weapon/LMWeapon.h"
#include "Weapon/LMParticularWeapon.h"

//////////////////////////////////////////////////////////////////////////
// ALonelyMenCharacter

ALonelyMenCharacter::ALonelyMenCharacter()
{


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	bWantsToFire = false;
	bEquipParticularWeapon = false;
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

void ALonelyMenCharacter::OnStartParticularFire()
{
	if (!bEquipParticularWeapon)
	{
		ParticularWeapon->OnEquip();
		bEquipParticularWeapon = true;
	}
	if (ParticularWeapon != nullptr)
	{
		ParticularWeapon->StartFire();
	}
}

void ALonelyMenCharacter::OnStopParticularFire()
{
	if (bEquipParticularWeapon)
	{
		ParticularWeapon->OnUnEquip();
		bEquipParticularWeapon = false;
	}
	if (ParticularWeapon != nullptr)
	{
		ParticularWeapon->StopFire();
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
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
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
		if (Cast<ALMParticularWeapon>(weapon) != NULL)
			this->ParticularWeapon = Cast<ALMParticularWeapon>(weapon);
		else
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

FName ALonelyMenCharacter::GetParticularWeaponAttachPoint() const
{
	return ParticularAttachPoint;
}

void ALonelyMenCharacter::StopAllAnimMontage()
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}

void ALonelyMenCharacter::SetRagDollPhysics()
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

float ALonelyMenCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate /* = 1.f */, FName StartSectionName /* = NAME_None */)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}
	return 0.0f;
}

void ALonelyMenCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent *UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance && UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOutTime);
	}
}

float ALonelyMenCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ALonelyMenCharacter::PlayDie(float KillingDamge, FDamageEvent const& DamageEvent, AController* killer, AActor* DamageCauser)
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

void ALonelyMenCharacter::PlayHit(float DamageTaken, struct FDamageEvent const & DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	USkeletalMeshComponent* UseMesh = GetMesh();

	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		float HitAnimDuration = UseMesh->AnimScriptInstance->Montage_Play(HitAnim);

		GetCharacterMovement()->Velocity = FVector::ZeroVector;

	}
}

void ALonelyMenCharacter::DestroyAllInventory()
{

}