// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/LMWeapon.h"
#include "LMWeapon_Instant.generated.h"

/**
 * 
 */
UCLASS()
class LONELYMEN_API ALMWeapon_Instant : public ALMWeapon
{
	GENERATED_BODY()
	
	virtual EAmmoType GetAmmoType() const override
	{
		return ALMWeapon::EAmmoType::EAT_Bullet;
	}
	

	virtual void FireWeapon() override;
};
