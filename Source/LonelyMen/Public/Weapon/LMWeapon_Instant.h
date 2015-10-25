// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/LMWeapon.h"
#include "LMWeapon_Instant.generated.h"


USTRUCT()
struct FInstantWeaponData
{
	GENERATED_USTRUCT_BODY()

	/* weapon attack ranage */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	float WeaponRanage;

	/* weapon attack damage */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	int32 WeaponDamage;

	/* weapon Damage type */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
		TSubclassOf<UDamageType> DamageType;

	FInstantWeaponData()
	{
		WeaponDamage = 100;
		WeaponRanage = 400;
		DamageType = UDamageType::StaticClass();    
	}
};
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
	
protected:
	UPROPERTY(EditDefaultsOnly, Category=Config)
	FInstantWeaponData InstantConfig;

	virtual void FireWeapon() override;

private:
	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);
	bool SholdDealDamage(AActor* TestActor);
};
