// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LMWeapon.generated.h"

namespace EWeaponState
{
	enum Type
	{
		EWS_Idle,
		EWS_Firing,
		EWS_Reloading,
		EWS_Equipping,
	};
}

UCLASS(Abstract)
class LONELYMEN_API ALMWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMWeapon();

protected:

	//////////////////////////////////////////////////////////////////////////
	// weapon usage

	/** current weapon state */
	EWeaponState::Type CurrentState;
	
	/** update weapon state */
	void SetWeaponState(EWeaponState::Type NewState);

	/** determine current weapon state */
	void DetermineWeaponState();

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip();

	/** weapon is now equipped by owner pawn */
	virtual void OnEquipFinished();

	/** weapon is holstered by owner pawn */
	virtual void OnUnEquip();



	//////////////////////////////////////////////////////////////////////////
	// Input 

	/** start weapon fire */
	virtual void StartFire();

	/** stop weapon fire */
	virtual void StopFire();

	/** start weapon reload */
	virtual void StartReload();

	/** interrupt weapon reload */
	virtual void StopReload();

	/** check if weapon can fire */
	bool CanFire() const;

	/** check if weapon can be reloaded */
	bool CanReload() const;

	//////////////////////////////////////////////////////////////////////////
	// weapon usage helpers
	/** find hit */
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;
};
