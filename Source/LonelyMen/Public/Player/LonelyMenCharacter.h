// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "LonelyMenCharacter.generated.h"

UCLASS(Abstract,config = Game)
class ALonelyMenCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	ALonelyMenCharacter();

#pragma region 武器相关
protected:
	/** currenly equipped weapon */
	UPROPERTY(Transient)
	class ALMWeapon* CurrentWeapon;

	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName WeaponAttachPoint;

	/** current equipped particular weapon */
	UPROPERTY()
	class ALMParticularWeapon* ParticularWeapon;

	/** socket or bone name for attaching particular weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName ParticularAttachPoint;

	/** current firing state */
	uint8 bWantsToFire : 1;

	/** Particular Weapon is equip */
	uint8 bEquipParticularWeapon : 1;

	/** which sub class of ALMWeapon can use 筛选出ALMWeapon的子类 */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class ALMWeapon>> DefaultInventoryClasses;

	UPROPERTY(Transient)
		TArray<class ALMWeapon*> Inventory;

	/** get firing state */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
		bool IsFiring() const;

	/** 创建默认的武器 */
	void SpawnDefaultInventory();

	/** 将武器初始化并加入武器库列表 */
	void AddWeapon(class ALMWeapon* weapon);

	/** 将武器从武器库列表内移除 */
	void RemoveWeapon(class ALMWeapon* weapon);

	/** 装备武器到角色身上 */
	void EquipWeapon(class ALMWeapon* weapon);

	/**  设置新武器为当前武器 */
	void SetCurrentWeapon(class ALMWeapon* NewWeapon, class ALMWeapon* LastWeapon = NULL);
	
	/** 移除所有武器*/
	void DestroyAllInventory();
public:
	/** get weapon attach point */
	FName GetWeaponAttachPoint() const;

	/** get particular weapon attach point */
	FName GetParticularWeaponAttachPoint() const;

#pragma endregion 武器相关

#pragma region 动画相关
public:
	/* check is Dying */
	uint32 bIsDying : 1;

	/* die anim */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage *DeathAnim;

	/* hit anim */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage *HitAnim;

	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

	void PlayHit(float DamageTaken, struct FDamageEvent const & DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);
	void PlayDie(float KillingDamge, FDamageEvent const& DamageEvent, AController* killer, AActor* DamageCauser);

	void StopAllAnimMontage();
	void SetRagDollPhysics();
#pragma endregion 动画相关

#pragma region 属性相关
protected:
	int nCurHealth;
	int nMaxHealth;
#pragma endregion 属性相关
public:
	/** player pressed start fire action */
	void OnStartFire();

	/** player released start fire action */
	void OnStopFire();

	/** player pressed start Particular fire action */
	void OnStartParticularFire();

	/** player pressed start particular fire action */
	void OnStopParticularFire();
protected:
	// APawn interface
	/** spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	// End of APawn interface
	

};

