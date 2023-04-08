// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPawn.h"
#include "Kismet/GameplayStatics.h"
#include "PacManPawn.h"

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
		if (EStates == Frightened)
		PacMan->Eat(this);
	
	}
}

void AGhostPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

void AGhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr && EStates != Frightened)
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

	const FVector Dimensions(60, 60, 20);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords()), true);
	}
}

void AGhostPawn::EnterFrightenedState()
{
	//APacManNode* RandomNode = nullptr;
	//double RandX = rand();
	//double RandY = rand();
	//RandomNode->SetNodePosition(RandX, RandY);
	//SetTargetNode(RandomNode);

	EStates = Frightened;


}

void AGhostPawn::TeleportToHome()
{
	const FVector Location(2050.0f, 1450.0f, GetActorLocation().Z);

	//Set my data_structure variables for the new node
	CurrentGridCoords = FVector2D(20.14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(20,14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(20,14))));

	SetTargetNode(NextNode);

	SetActorLocation(Location);
}
