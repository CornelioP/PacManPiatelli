// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGameMode.h"

APacManGameMode::APacManGameMode()
{

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

	//BlinkyP = GetWorld()->SpawnActor<ABlinky>(Blinky, FVector(2750.0f,2650.0f, 30.0f), FRotator(0, 0, 0));
}