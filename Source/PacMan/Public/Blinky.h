// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Blinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API ABlinky : public AGhostPawn
{
	GENERATED_BODY()

private:

	FVector2D BlinkyScatterNodeCoord;

	APacManNode* BlinkyScatterNode;
	
public:

	ABlinky();

	virtual void BeginPlay() override;

	virtual void SetGhostTarget() override;

	virtual APacManNode* GetPlayerRelativeTarget() override;

	virtual void Tick(float DeltaTime) override;

	virtual void TeleportToHome() ;

	void ElroyEnter();

	//Bool to check if is Elroy

	UPROPERTY(EditAnywhere)
		bool IsElroy;

};
