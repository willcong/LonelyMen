// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMParticularWeapon.h"


void ALMParticularWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		DetachMeshFromPawn();

		FName AttachPoint = MyPawn->GetParticularWeaponAttachPoint();

		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		Mesh->SetHiddenInGame(false);
		Mesh->AttachTo(PawnMesh, AttachPoint);
	}
}

void ALMParticularWeapon::DetachMeshFromPawn()
{
	Mesh->SetHiddenInGame(true);
	Mesh->DetachFromParent();
}

void ALMParticularWeapon::FireWeapon()
{

}