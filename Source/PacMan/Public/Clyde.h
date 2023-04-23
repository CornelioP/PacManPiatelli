// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Clyde.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AClyde : public AGhostPawn
{
	GENERATED_BODY()
	
public:

	AClyde();

	virtual void Tick(float DeltaTime) override;

	virtual void TeleportToHome();

	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;

private:

	FVector2D ClydeScatterNodeCoord;

	APacManNode* ClydeScatterNode;

};
