// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

ABlinky::ABlinky()
{
	CurrentGridCoords = FVector2D(27, 26);
}

APacManNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}
