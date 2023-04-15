// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPawn.h"
#include "Kismet/GameplayStatics.h"
#include "PacManPawn.h"
#include "PacManGameMode.h"

AGhostPawn::AGhostPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];
	Player = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacManPawn::StaticClass()));
  
	SetSpeed(300);
}

void AGhostPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	const auto PacMan = Cast<APacManPawn>(OtherActor);
	if (PacMan)
	{
		//When in Frightened state eat the Ghost
		if (GameMode->EStates == Frightened)
		PacMan->Eat(this);

		else
		{   
			//If not in Frightened mode Ghost kill Pacman
			PacMan->Respawn();
		}
	}
}

void AGhostPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

void AGhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr && GameMode->EStates != Frightened)
	{
		SetGhostTarget();
	}
}

APacManPawn* AGhostPawn::GetPlayer() const
{
	
		return Player;
	
}

void AGhostPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

APacManNode* AGhostPawn::GetPlayerRelativeTarget()
{
	 return Player->GetLastNode();;
}

void AGhostPawn::SetGhostTarget()
{   

	const APacManNode* Target = GetPlayerRelativeTarget();
	
	if (!Target)
	{
		Target = GetPlayer()->GetLastNode();
	}

	APacManNode* PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));


	if (PossibleNode)
	{
		this->SetNextNodeByDir(MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords()), true);
	}
}


void AGhostPawn::TeleportToGhostBase()
{
	const FVector GhostBase(1750.0f, 1450.0f, 51.0f);

	CurrentGridCoords = FVector2D(17, 14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(17, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(17, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(GhostBase);
}
