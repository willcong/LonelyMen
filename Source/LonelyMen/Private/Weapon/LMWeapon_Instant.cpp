// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMWeapon_Instant.h"




void ALMWeapon_Instant::FireWeapon()
{
	const int TraceMaxDistance = 2000;
	const FVector AimDir = GetAdjustedAim();
	const FVector StartTrace = GetMuzzleLocation();
	const FVector EndTrace = StartTrace + AimDir * TraceMaxDistance;

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);


	//这里处理击中目标的效果 Begin
	//这里处理击中目标的效果 End
}