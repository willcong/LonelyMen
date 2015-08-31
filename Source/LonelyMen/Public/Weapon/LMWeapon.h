// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

UCLASS(Abstract, Blueprintable)
class ALMWeapon : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

protected:
	UPROPERTY(Transient)
	class ALonelyMenCharacter* MyPawn;


public:
	/** set the weapon's owning pawn */
	void SetOwningPawn(ALonelyMenCharacter* NewOwner);

	//////////////////////////////////////////////////////////////////////////
	// Ammo
	enum class EAmmoType
	{
		EAT_None,
		EAT_Bullet,
		EAT_Rocket,
		EAT_Max,
	};

	void GiveAmmo(int AddMount);

	void UseAmmo();

	virtual EAmmoType GetAmmoType()const
	{
		return EAmmoType::EAT_None;
	}

	/** current total ammo */
	UPROPERTY()
	int32 CurrentAmmo;
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

	/** weapon add to owner Inventory */
	virtual void OnEnterInventory(ALonelyMenCharacter* owner);

	/** weapon remove form owner Inventory */
	virtual void OnLeaveInventory();

	/** Get the aim of the weapon, allowing for adjustments to be made by the weapon */
	virtual FVector GetAdjustedAim() const;

	/**  name of bone/socket for muzzle in weapon mesh  */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName MuzzleAttachName;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	/** get weapon mesh (needs pawn owner to determine variant) */
	USkeletalMeshComponent* GetWeaponMesh() const;

	/** get the muzzle location of the weapon */
	FVector GetMuzzleLocation() const;

	/** get direction of weapon's muzzle */
	FVector GetMuzzleDirection() const;

	/** attach weapon mesh to owner pawn */
	virtual void AttachMeshToPawn();

	/** detach weapon mesh from owner pawn */
	virtual void DetachMeshFromPawn();

	/** 是否连发 */
	bool IsTriggerBursts;

	/** 连发速度 */
	UPROPERTY()
	float FireBurstsRate;

	float LastFireBurstsTime;
	//////////////////////////////////////////////////////////////////////////
	// Input 

	/** start weapon fire */
	virtual void StartFire();

	/** stop weapon fire */
	virtual void StopFire();

	/**  start particular weapon fire*/
	virtual void StartParticularFire(){}

	/** stop weapon fire */
	virtual void StopParticularFire(){}

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
	virtual void FireWeapon() PURE_VIRTUAL(AShooterWeapon::FireWeapon, );

	//////////////////////////////////////////////////////////////////////////
	// weapon animation
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimationAsset* FireAnim;

public:
	void PlayWeaponAnimation(class UAnimationAsset* newAnim);
protected:
	virtual void Tick(float DeltaSeconds) override;

};
