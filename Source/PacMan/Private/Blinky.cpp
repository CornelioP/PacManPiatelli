// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"
#include "PacManGameMode.h"

ABlinky::ABlinky()
{
	IsElroy = false;
}

APacManNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

void ABlinky::BeginPlay()
{
	AGhostPawn::BeginPlay();

	BlinkyScatterNodeCoord = FVector2D(35, 25);

	BlinkyScatterNode = *(MazeGen->TileMap.Find(BlinkyScatterNodeCoord));
}

void ABlinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//There are 234 points, enabled when there are 20 points left 
	if (GameMode->PointCounter == 214  || GameMode->PointCounter == 224)
	{
		ElroyEnter();
	}
}

void ABlinky::SetGhostTarget()
{
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase || IsElroy)) {

			Target = GetPlayerRelativeTarget();

			if (!Target)
			{
				Target = GetPlayer()->GetLastNode();
			}
		}
		else
		{
			Target = BlinkyScatterNode;
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

void ABlinky::TeleportToHome()
{

	const FVector BlinkySpawn(2050.0f, 1450.0f, 51.0f);

	CurrentGridCoords = FVector2D(20, 14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(20, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(20, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(BlinkySpawn);
}

void ABlinky::ElroyEnter()
{
	IsElroy = true;

	if (GameMode->PointCounter == 214)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("EELLLLRROOOOYYY 320")));
	    CurrentMovementSpeed = 320;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("EELLLLRROOOOYYY 400")));
	    CurrentMovementSpeed = 400;
	}

}


