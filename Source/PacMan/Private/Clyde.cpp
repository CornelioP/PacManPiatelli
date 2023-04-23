// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"
#include "PacManGameMode.h"
#include "Math/Vector.h"

AClyde::AClyde()
{
}

void AClyde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClyde::TeleportToHome()
{
}

void AClyde::SetGhostTarget()
{
	
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase)) {

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"),(FVector::Dist(this->GetActorLocation(), GetPlayer()->GetActorLocation()))));
			
			//Calc euclide distance from Pacman ,if more than 8 tiles target Pacman, else target the scatter mode node
			if (FVector::Dist(this->GetActorLocation(), GetPlayer()->GetActorLocation()) > 850)
			{
				Target = GetPlayerRelativeTarget();

				if (!Target)
				{
					Target = GetPlayer()->GetLastNode();

				}
			}
			else
			{
				Target = ClydeScatterNode;
			}
		}
		else
		{
			Target = ClydeScatterNode;
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

void AClyde::BeginPlay()
{
	AGhostPawn::BeginPlay();

	ClydeScatterNodeCoord = FVector2D(2, 0);

	ClydeScatterNode = *(MazeGen->TileMap.Find(ClydeScatterNodeCoord));
}
