// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManPawn.h"
#include "PacManGameMode.h"
#include "Kismet/GameplayStatics.h"

APacManPawn::APacManPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	LastInputDirection = FVector(0,0,0);
	LastValidInputDirection = FVector(0,0,0);

	CurrentGridCoords = FVector2D(8,14);

	NormalMovementSpeed = (StandardSpeed/100)*80;

	//Fruit should spawn near the center of the Maze 
	minX = 6;
	maxX = 21;

	minY = 11;
	maxY = 26;

	Spawnable = false;
	
}
void APacManPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];

	LifeCounter = 2.0;
	PointCounter = 0.0;

}

void APacManPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove autonomamente fino a che il next node è walkable
	if (!TargetNode && !NextNode)
	{
		if (MazeGen->IsNodeValidForWalk(MazeGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}

	//if (TargetNode == GhostNode)
	//{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("FUNZIONA PORCODIO ?????")));
		//SetTargetNode(nullptr);
		//SetNextNode(nullptr);

	//}
}


void APacManPawn::SetTargetNode(APacManNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacManPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Point = Cast<APointActor>(OtherActor);
	const auto PowerPoint = Cast<APowerPoint>(OtherActor);
	const auto FruitCast = Cast<AFruitActor>(OtherActor);

	if (Point)
	{
		//Hide actor and disable his tick  
		Point->HideInGame = true;
		Point->DisableActor();

		//Increment point when PacMan collide with PointActor
	    PointCounter += 1;
	    
		//Slow down for 1/60 of second
		
		//float PointEatenTime = 0.01666;

		//Slowdown PacMan at 71% if in Normal mode
		//if (PacManStates == Normal)
		//{
		//	this->CurrentMovementSpeed = 284;
		//	GetWorld()->GetTimerManager().SetTimer(FrightnedTimer, this, &APacManPawn::PointEatenExit, PointEatenTime, false);
		//}

		//SlowDown PacMan at 79% if in Frightened mode 
		//else if (PacManStates == FrightenedP)
		//{
		//	this->CurrentMovementSpeed = 316;
		//	GetWorld()->GetTimerManager().PauseTimer(FrightnedTimer);
		//	GetWorld()->GetTimerManager().SetTimer(FrightnedTimer, this, &APacManPawn::PointEatenExit, PointEatenTime, false);
		//}


			//When point counter reach max call end game 
			//if (PointCounter == 237)
			//{
			//	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Background, false);
			//}
	}

	if (PowerPoint)
	{
		PowerPoint->HideInGame = true;
		PowerPoint->DisableActor();
		PowerPoint->IsEaten = true;

		//When a power point is eaten enter Frightened mode 

		GameMode->EnterFrightenedMode();
	}

	if (FruitCast)
	{
		Fruit->HideInGame = true;
		Fruit->DisableActor();
	}


}

void APacManPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = MazeGen->GetNextNode(CurrentGridCoords, Dir);
	if (MazeGen->IsNodeValidForWalkPacMan(Node))
	{
		SetTargetNode(Node);
	}
}

void APacManPawn::SetNextNodeByDir(FVector InputDir, bool ForceLast)
{

	const FVector2D Coords = TargetNode ? TargetNode->GetNodePosition() : LastNode->GetNodePosition();
	const auto Node = GameMode->GMaze->GetNextNode(Coords, InputDir);
	if (GameMode->GMaze->IsNodeValidForWalkPacMan(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

void APacManPawn::Eat(AGhostPawn* Ghost)
{ 
	//When a ghost is eaten teleport it to ghost's house
	const FVector GhostBase = FVector(1450.0f, 1250.0f, 51.0f);
	Ghost->IsEaten = true;
}





void APacManPawn::PointEatenExit()
{
	//if(PacManStates == Normal)
	 //  this->CurrentMovementSpeed = 320;
	//else if (PacManStates == FrightenedP)
	//	this->CurrentMovementSpeed = 360;
}

void APacManPawn::RespawnPacMan()
{
	//Decrease life counter

	//Teleport PacMan to spawn point

	const FVector Spawn = FVector(850.0f, 1450.0f, GetActorLocation().Z);

	CurrentGridCoords = FVector2D(8,14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(8, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(8, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(Spawn);
 	
}

void APacManPawn::SpawnFruit()
{

	while (!Spawnable)
	{

	 int32 Randx = int32(FMath::FRandRange(minX, maxX));
	 int32 Randy = int32(FMath::FRandRange(minY, maxY));

	 FVector2D FruitCoord = FVector2D(Randx, Randy);
	
	 APacManNode* SpawnFruitNode = *(MazeGen->TileMap.Find(FruitCoord));

	 FVector Position = MazeGen->GetRelativeLocationByXYPosition(Randx,Randy);

	 Spawnable = MazeGen->IsNodeValidForWalk(SpawnFruitNode);

	 Position.X += 50;
	 Position.Y += 50;

	   if (Spawnable)
	    {
		  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("FRUTTA SPAWNATA")));
	      Fruit = GetWorld()->SpawnActor<AFruitActor>(FruitActor, Position, FRotator::ZeroRotator);
	    }
	}
	

}

void APacManPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PointCounter == 70 || PointCounter == 170)
	{
		SpawnFruit();
	}
}

void APacManPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection,true);
}

void APacManPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection,true);
}

void APacManPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveY"), this, &APacManPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveX"), this, &APacManPawn::SetHorizontalInput);
}
