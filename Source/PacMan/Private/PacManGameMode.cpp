// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGameMode.h"
#include "Kismet/GameplayStatics.h"

APacManGameMode::APacManGameMode()
{
	
	LifeCounter = 2.0;
	PointCounter = 0.0;
    
	
}

void APacManGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector GridPos(50.0f, 50.0f, 50.0f);


	if (MazeGeneratorClass != nullptr)
	{
		//Spawn Maze
		GMaze = GetWorld()->SpawnActor<AMazeGenerator>(MazeGeneratorClass, GridPos, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	};

	Blinky = Cast<ABlinky>(UGameplayStatics::GetActorOfClass(GetWorld(),BlinkyClass));

	Inky = Cast<AInky>(UGameplayStatics::GetActorOfClass(GetWorld(), InkyClass));

	PacMan = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), PacManClass));

	EnterScatterMode();

}


//Finite state machine handler 
void APacManGameMode::EnterFrightenedMode()
{

	//Save last state

	if (EStates == Chase)
	{
		LastState = 0;
		GetWorld()->GetTimerManager().PauseTimer(ChaseTimer);

	}

	if (EStates == Scatter)
	{
		LastState = 1;
		GetWorld()->GetTimerManager().PauseTimer(ScatterTimer);

	}

	//Set current state to frightened

	EStates = Frightened;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ENTRATO IN STATO FRIGHTENED")));

	//Initialize timer 
	float FrightenedTime = 6.0;
	GetWorld()->GetTimerManager().SetTimer(FrightnedTimer, this, &APacManGameMode::FrightenedExit, FrightenedTime, false);

	//Set Movement Speed

	PacMan->CurrentMovementSpeed = 360;

	Blinky->GhostFrightenedState();

	Inky->GhostFrightenedState();

}

void APacManGameMode::FrightenedExit()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("USCITO STATO FRIGHTENED")));

	//Back to the last state
	if (LastState == 0)
	{
		EnterChaseMode();

	}

	if (LastState == 1)
	{
	 
		EnterScatterMode();

	}
	
}

void APacManGameMode::EnterChaseMode()
{

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ENTRATO STATO CHASE")));

	//Increment Chase Counter

	Chase_Counter += 1;

	//Initialize timer 
	float ChaseTime = 20.0;

	//After the third time it stays in Chase state

	if (Chase_Counter == 1 || Chase_Counter == 2 || Chase_Counter == 3)
	{
	GetWorld()->GetTimerManager().SetTimer(ChaseTimer, this, &APacManGameMode::ChaseExit, ChaseTime, false);
	}

	//Set current state to Chase

	EStates = Chase;

	//Set movement speed

	PacMan->CurrentMovementSpeed = 320;

	Inky->CurrentMovementSpeed = 300;

    Blinky->CurrentMovementSpeed = 300;
	
}

void APacManGameMode::ChaseExit()
{
	EnterScatterMode();
}

void APacManGameMode::EnterScatterMode()
{

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ENTRATO STATO SCATTER")));

	//Increment Scatter Counter
	
	Scatter_Counter += 1;

	//Initialize timer 
	float ScatterTime;

	//First two times the scatter state is enabled for 7 second, after that is 5 seconds

	if (Scatter_Counter == 1 || Scatter_Counter == 2)
	{
		ScatterTime = 7.0;
	}
	else
	{
		ScatterTime = 5;
	}
	GetWorld()->GetTimerManager().SetTimer(ScatterTimer, this, &APacManGameMode::ScatterExit, ScatterTime, false);

	//Set current state to Scatter

	EStates = Scatter;

	//Scatter mode movements is handled in the SetGhostTarget methods of each ghost
	
}

void APacManGameMode::ScatterExit()
{
	EnterChaseMode();
}

