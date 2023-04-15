// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

ABlinky::ABlinky()
{
	

}

APacManNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

void ABlinky::TeleportToHome()
{

	const FVector BlinkySpawn(2050.0f, 1450.0f, 51.0f);

	CurrentGridCoords = FVector2D(20, 14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(20, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(20, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(BlinkySpawn);
}
