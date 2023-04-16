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

	NormalMovementSpeed = 320;
}
void APacManPawn::BeginPlay()
{
	Super::BeginPlay();
	//// posizione iniziale del pawn (è quella del PlayerStart)
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];

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

	if (Point)
	{
		//Hide actor and disable his tick  
		Point->HideInGame = true;
		Point->DisableActor();

		//Increment point when PacMan collide with PointActor
	    GameMode->PointCounter += 1;
	    
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

		//When a power point is eaten enter Frightened mode 

		GameMode->EnterFrightenedMode();
	}


}

void APacManPawn::Eat(AGhostPawn* Ghost)
{ 
	//When a ghost is eaten teleport it to ghost's house
	const FVector GhostBase = FVector(1450.0f, 1250.0f, 51.0f);
	Ghost->TeleportToGhostBase();
}





void APacManPawn::PointEatenExit()
{
	//if(PacManStates == Normal)
	 //  this->CurrentMovementSpeed = 320;
	//else if (PacManStates == FrightenedP)
	//	this->CurrentMovementSpeed = 360;
}

void APacManPawn::Respawn()
{
	//Decrease life counter



	//Teleport PacMan to spawn point

	const FVector Spawn = FVector(850.0f, 1450.0f, GetActorLocation().Z);

	CurrentGridCoords = FVector2D(8,14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(8, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(8, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(Spawn);

	//Teleport Ghosts to Spawn Point
 	





}

void APacManPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APacManPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);
}

void APacManPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
}

void APacManPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveY"), this, &APacManPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveX"), this, &APacManPawn::SetHorizontalInput);
}
