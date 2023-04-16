// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacManGameMode.h"
#include "PacManPawn.h"


AInky::AInky()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameMode->PointCounter == 30)
	{
		CanMove = true;

		OutOfGhostHouse();
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

void AInky::SetGhostTarget()
{
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	if (GameMode->EStates != Frightened)
	{
		if (GameMode->EStates == Chase ) {

			Target = GetPlayerRelativeTarget();

			if (!Target)
			{
				Target = GetPlayer()->GetLastNode();
			}
		}
		else
		{
			Target = InkyScatterNode;
		}

		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));

	}

	else
	{
		SetFrightenedStateTarget();
		PossibleNode = TargetNode;
	}

	if (PossibleNode)
	{
		TargetNode = PossibleNode;
		NextNode = PossibleNode;
		LastValidInputDirection = MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords());
	}
}

void AInky::BeginPlay()
{
	AGhostPawn::BeginPlay();

	//Start moving when 30 point are reached
	CanMove = false;

	InkyScatterNodeCoord = FVector2D(2, 27);

	InkyScatterNode = *(MazeGen->TileMap.Find(InkyScatterNodeCoord));
}
