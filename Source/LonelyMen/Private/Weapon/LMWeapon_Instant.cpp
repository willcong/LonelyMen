// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMWeapon_Instant.h"




void ALMWeapon_Instant::FireWeapon()
{
	const FVector AimDir = GetAdjustedAim();
	const FVector StartTrace = GetMuzzleLocation();
	const FVector EndTrace = StartTrace + AimDir * InstantConfig.WeaponRanage;

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);


	//这里处理击中目标的效果 Begin
	DealDamage(Impact, AimDir);
	//这里处理击中目标的效果 End
}

bool ALMWeapon_Instant::SholdDealDamage(AActor* TestActor)
{
	bool res = false;

	if (Cast<ANonPlayerCharacter>(TestActor))
		res = true;

	return res;
}

void ALMWeapon_Instant::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	if (!Impact.GetActor() || !SholdDealDamage(Impact.GetActor()))
		return;

	FPointDamageEvent PointDamage;
	PointDamage.DamageTypeClass = InstantConfig.DamageType;
	PointDamage.HitInfo = Impact;
	PointDamage.ShotDirection = ShootDir;
	PointDamage.Damage = InstantConfig.WeaponDamage;

	Impact.GetActor()->TakeDamage(PointDamage.Damage, PointDamage, MyPawn->Controller, this);
}