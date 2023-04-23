// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Pinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APinky : public AGhostPawn
{
	GENERATED_BODY()

public:

	APinky();

	virtual void Tick(float DeltaTime) override;


	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;

private:

	FVector2D PinkyScatterNodeCoord;

	APacManNode* PinkyScatterNode;

	FVector2D PinkySpawn = FVector2D(0, 0);
	
};
