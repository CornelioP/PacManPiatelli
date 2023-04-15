// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Inky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AInky : public AGhostPawn
{
	GENERATED_BODY()
public:	
	virtual void TeleportToHome();

protected:
	virtual void OnNodeReached() override;
};
