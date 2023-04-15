// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacManGameMode.h"
#include "PacManPawn.h"

void AInky::OnNodeReached()
{
    if (GameMode->PointCounter >= 30.0)
    {
        Super::OnNodeReached();
    }
}

void AInky::TeleportToHome()
{
	const FVector InkySpawn(1750.0f, 1250.0f, 51.0f);

	CurrentGridCoords = FVector2D(17, 12);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(17, 12)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(17, 12))));

	SetTargetNode(NextNode);

	SetActorLocation(InkySpawn);
}
