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

	GetWorld()->SpawnActor<ABlinky>(Blinky, FVector(27550.0f,26550.0f, 50.0f), FRotator(0, 0, 0));
}