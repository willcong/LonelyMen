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


	//���ﴦ�����Ŀ���Ч�� Begin
	//���ﴦ�����Ŀ���Ч�� End
}