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
	SetSpeed(400);
}

void AGhostPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	const auto PacMan = Cast<APacManPawn>(OtherActor);
	if (PacMan)
	{
		//PN->Eat();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("I Reached you..")));
	}
}

void AGhostPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

void AGhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
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
	DrawDebugBox(GetWorld(), PossibleNode->GetNodeCoordinates(), Dimensions, FColor::Red);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords()), true);
	}
}
