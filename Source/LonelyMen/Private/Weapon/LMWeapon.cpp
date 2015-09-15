// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMWeapon.h"
#include "Public/DrawDebugHelpers.h"


// Sets default values
ALMWeapon::ALMWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentAmmo = 0;
	Mesh = ObjectInitializer.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->bChartDistanceFactor = true;
	Mesh->bReceivesDecals = true;
	Mesh->CastShadow = true;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	RootComponent = Mesh;

	FireBurstsRate = 0.1f;
}


void ALMWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	
}

void ALMWeapon::DetermineWeaponState()
{

}

void ALMWeapon::OnEquip()
{
	AttachMeshToPawn();
}

void ALMWeapon::OnEquipFinished()
{}

void ALMWeapon::OnUnEquip()
{
	DetachMeshFromPawn();
}

void ALMWeapon::StartFire()
{
	IsTriggerBursts = true;
	FireWeapon();
	PlayWeaponAnimation(FireAnim);
	UseAmmo();
}

void ALMWeapon::StopFire()
{
	IsTriggerBursts = false;
}

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
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	{
		DrawDebugLine(GetWorld(), TraceFrom, TraceTo, FColor::Red,true,10.0f);
	}

	return Hit;

}

void ALMWeapon::GiveAmmo(int AddMount)
{
	CurrentAmmo += AddMount;
}

void ALMWeapon::UseAmmo()
{
	CurrentAmmo--;
}

void ALMWeapon::SetOwningPawn(ALonelyMenCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

FVector ALMWeapon::GetAdjustedAim() const
{
	FVector FinalAim = FVector::ZeroVector;

	if (MyPawn)
	{
		FRotator CamRot;

		CamRot = MyPawn->GetActorRotation();
		FinalAim = CamRot.Vector();
	}

	return FinalAim;
}

USkeletalMeshComponent* ALMWeapon::GetWeaponMesh() const
{
	return Mesh;
}

FVector ALMWeapon::GetMuzzleLocation() const
{
	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	if (UseMesh != nullptr)
	{
		return UseMesh->GetSocketLocation(MuzzleAttachName);
	}
	return FVector::ZeroVector;
}

FVector ALMWeapon::GetMuzzleDirection() const
{
	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	if (UseMesh != nullptr)
	{
		return UseMesh->GetSocketRotation(MuzzleAttachName).Vector();
	}

	return FVector::ZeroVector;
}

void ALMWeapon::OnEnterInventory(ALonelyMenCharacter* owner)
{
	SetOwningPawn(owner);
}

void ALMWeapon::OnLeaveInventory()
{
	SetOwningPawn(NULL);
}

void ALMWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		DetachMeshFromPawn();

		FName AttachPoint = MyPawn->GetWeaponAttachPoint();

		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		Mesh->SetHiddenInGame(false);
		Mesh->AttachTo(PawnMesh, AttachPoint);
	}
}

void ALMWeapon::DetachMeshFromPawn()
{ 
	Mesh->SetHiddenInGame(true);
	Mesh->DetachFromParent();
}

void ALMWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (IsTriggerBursts)
	{
		LastFireBurstsTime += DeltaSeconds;
		if (LastFireBurstsTime >= FireBurstsRate)
		{
			LastFireBurstsTime = 0;
			StartFire();
		}
	}
}

void ALMWeapon::PlayWeaponAnimation(class UAnimationAsset* newAnim)
{
	if (newAnim)
	{
		Mesh->PlayAnimation(newAnim,false);
	}
}