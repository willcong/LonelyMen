// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMWeapon.h"


// Sets default values
ALMWeapon::ALMWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ALMWeapon::SetWeaponState(EWeaponState::Type NewState)
{

}

void ALMWeapon::DetermineWeaponState()
{

}

void ALMWeapon::OnEquip()
{}

void ALMWeapon::OnEquipFinished()
{}

void ALMWeapon::OnUnEquip()
{}

void ALMWeapon::StartFire()
{}

void ALMWeapon::StopFire()
{}

void ALMWeapon::StartReload()
{}

void ALMWeapon::StopReload()
{}

bool ALMWeapon::CanFire() const
{
	return true;
}

bool ALMWeapon::CanReload() const
{
	return true;
}

FHitResult ALMWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;

}