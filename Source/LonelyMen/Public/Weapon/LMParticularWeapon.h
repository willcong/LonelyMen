// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/LMWeapon.h"
#include "LMParticularWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LONELYMEN_API ALMParticularWeapon : public ALMWeapon
{
	GENERATED_BODY()
	
public:
	/** attach weapon mesh to owner pawn */
	virtual void AttachMeshToPawn() override;

	/** detach weapon mesh from owner pawn */
	virtual void DetachMeshFromPawn() override;
	
	virtual void FireWeapon() override;
};
