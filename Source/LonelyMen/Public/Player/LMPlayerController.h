// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "LMPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LONELYMEN_API ALMPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALMPlayerController();
private:
	///////////////////////////调整角色面向
	bool bIsRotationChange;
	void RotationChange();
	void StopRotationChange();
	void PawnRotationToTarget();
	///////////////////////////////////////选择目标对象
	UPrimitiveComponent *pCurSelectedComponent = nullptr;
	ALonelyMenCharacter *pMyCharacter = nullptr;//当前控制的Character
	//////////////////////////////////////////////////////////////////////////Character开火攻击
	void OnStartFire();
	void OnStopFire();
	void OnStartParticularFire();
	void OnStopParticularFire();
public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void RMouseDownSelectTarget();
protected:
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds);
	virtual void Possess(APawn* aPawn) override;

};
