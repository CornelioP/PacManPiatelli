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
	
public:
	ABlinky();

	virtual APacManNode* GetPlayerRelativeTarget() override;

};
