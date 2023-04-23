// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPawn.h"
#include "Kismet/GameplayStatics.h"
#include "PacManPawn.h"
#include "PacManGameMode.h"

AGhostPawn::AGhostPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh eyes
	StaticMeshLeftEye = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left eye"));
	StaticMeshRightEye = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right eye"));
	StaticMeshLeftEyePupil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left eye Pupil"));
	StaticMeshRightEyePupil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right eye Pupil"));

	//Mesh to handle frightened state 
	StaticMeshBlue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blue Mesh"));
	StaticMeshWhite = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("White Mesh"));

	StaticMeshLeftEyePupil->SetupAttachment(Collider);
	StaticMeshRightEyePupil->SetupAttachment(Collider);
	StaticMeshLeftEye->SetupAttachment(Collider);
	StaticMeshRightEye->SetupAttachment(Collider);
	StaticMeshBlue->SetupAttachment(Collider);
	StaticMeshWhite->SetupAttachment(Collider);

	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	//Time in blue and white 
	WhiteTime = 1.0;
	BlueTime = 1.0;
	FlashCounter = 0;

	IsEaten = false;

}

void AGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];
	Player = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacManPawn::StaticClass()));


  
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
			GameMode->Respawn();
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

void AGhostPawn::GhostFrightenedState()
{
	//In frightened state Ghost go at 50% speed
	FrightenedSpeed();

	//Make Ghosts Flash
	MeshHandler();

	//Invert direction to singal frightened state entered
	InvertDirection();


}

void AGhostPawn::FrightenedSpeed()
{
	this->CurrentMovementSpeed = (StandardSpeed / 100) * 50;
}

void AGhostPawn::ChaseScatterSpeed()
{
	this->CurrentMovementSpeed = (StandardSpeed / 100) * 75;
}

void AGhostPawn::InvertDirection()
{
	LastValidInputDirection = LastValidInputDirection * (-1);
}

void AGhostPawn::MeshHandler()
{
	StaticMeshBlue->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::MeshWhite, BlueTime, false);
}

void AGhostPawn::MeshWhite()
{
	StaticMeshWhite->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(WhiteMeshTimer, this, &AGhostPawn::MeshBlue, WhiteTime, false);
}

void AGhostPawn::MeshBlue()
{
	//Increment FlashCounter 
	FlashCounter += 1;
	//Set mesh blue
	StaticMeshWhite->SetVisibility(false);

	if (FlashCounter < 3)
	{
		GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::MeshWhite, BlueTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::ResetMesh, BlueTime, false);
	}
}

void AGhostPawn::ResetMesh()
{
	//Disable Mesh
	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	//Reset counter
	FlashCounter = 0;
}

void AGhostPawn::SetFrightenedStateTarget()
{
	float RandomNumber = Stream.FRandRange(0, 100);

	if (RandomNumber <= 25)
	{
		 DirCasualVector = FVector(1, 0, 0);
	}
	else if (RandomNumber> 25 && RandomNumber <=50)
	{
		DirCasualVector = FVector(0, 1, 0);
	}
	else if ( RandomNumber > 50 && RandomNumber <= 75)
	{
		 DirCasualVector = FVector(-1, 0, 0);
	}
	else if (RandomNumber > 75 && RandomNumber <= 100)
	{
		 DirCasualVector = FVector(0, -1, 0);
	}

	 RandomNodeCord = CurrentGridCoords + MazeGen->GetTwoDOfVector(DirCasualVector);
	 RandomNode = *(MazeGen->TileMap.Find(RandomNodeCord));

	 if (MazeGen->IsNodeValidForWalk(RandomNode))
	 {
		 TargetNode = RandomNode;
	 }
}

void AGhostPawn::OutOfGhostHouse()
{
	//APacManNode* OutHouseNode = *(MazeGen->TileMap.Find(FVector2D(20, 12)));

	//APacManNode* PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), OutHouseNode->GetNodePosition(), -(this->GetLastValidDirection()));

	//TargetNode = PossibleNode;

	//NextNode = PossibleNode;

	//LastValidInputDirection = MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords());
}

void AGhostPawn::RespawnGhost(FVector2D RespawnNode)
{
	StaticMesh->SetVisibility(false);
	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	TargetNode = *(MazeGen->TileMap.Find(RespawnNode));

}

