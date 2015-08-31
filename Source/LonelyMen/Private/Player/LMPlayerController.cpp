// Fill out your copyright notice in the Description page of Project Settings.

#include "LonelyMen.h"
#include "LMPlayerController.h"


ALMPlayerController::ALMPlayerController()
{
	this->bShowMouseCursor = true;
}

void ALMPlayerController :: RotationChange()
{
	this->bIsRotationChange = true;
}

void ALMPlayerController::StopRotationChange()
{
	this->bIsRotationChange = false;
}

void ALMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("ChangePlayerRotation",EInputEvent::IE_Pressed, this, &ALMPlayerController::RotationChange);
	InputComponent->BindAction("ChangePlayerRotation", EInputEvent::IE_Released, this, &ALMPlayerController::StopRotationChange);

	InputComponent->BindAction("SelectMouseDownTargetObject", EInputEvent::IE_Pressed, this, &ALMPlayerController::RMouseDownSelectTarget);

	//绑定攻击
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ALMPlayerController::OnStartFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ALMPlayerController::OnStopFire);
	InputComponent->BindAction("ParticularFire", EInputEvent::IE_Pressed, this, &ALMPlayerController::OnStartParticularFire);
	InputComponent->BindAction("ParticularFire", EInputEvent::IE_Released, this, &ALMPlayerController::OnStopParticularFire);
}

void ALMPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	PawnRotationToTarget();

}

void ALMPlayerController::PawnRotationToTarget()
{
	if (this->bIsRotationChange)
	{
		FHitResult CursorHitRes = FHitResult();
		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHitRes))
		{
			FVector FaceDir = CursorHitRes.Location - GetPawn()->GetActorLocation();
			FRotator FaceRotator = FaceDir.Rotation();
			FaceRotator.Pitch = 0;
			FaceRotator.Roll = 0;
			GetPawn()->SetActorRotation(FaceRotator);
		}
	}

}

void ALMPlayerController::RMouseDownSelectTarget()
{
	FHitResult HitRes = FHitResult();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	//只检测一下对象
	ObjectType.Add(EOBJECTTYPEQUERY_SELECTACTOR);

	if (GetHitResultUnderCursorForObjects(ObjectType,false,HitRes))
	{
		if (HitRes.GetComponent() == pCurSelectedComponent)
		{
			return;
		}
		else
		{
			if (pCurSelectedComponent != nullptr)
				pCurSelectedComponent->SetRenderCustomDepth(false);

			pCurSelectedComponent = HitRes.GetComponent();

			pCurSelectedComponent->SetRenderCustomDepth(true);

		}

	}
}

void ALMPlayerController::OnStartFire()
{
	RotationChange();
	if (pMyCharacter)
	{
		pMyCharacter->OnStartFire();
	}
}

void ALMPlayerController::OnStopFire()
{
	StopRotationChange();
	if (pMyCharacter)
	{
		pMyCharacter->OnStopFire();
	}
}

void ALMPlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	ALonelyMenCharacter *tmpCharacter = Cast<ALonelyMenCharacter>(aPawn);
	if (tmpCharacter != NULL)
	{
		this->pMyCharacter = tmpCharacter;
	}
}

void ALMPlayerController::OnStartParticularFire()
{
	if (pMyCharacter)
	{
		pMyCharacter->OnStartParticularFire();
	}
}

void ALMPlayerController::OnStopParticularFire()
{
	if (pMyCharacter)
	{
		pMyCharacter->OnStopParticularFire();
	}
}