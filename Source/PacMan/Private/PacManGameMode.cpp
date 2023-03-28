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
		// spawn della griglia
		GMaze = GetWorld()->SpawnActor<AMazeGenerator>(MazeGeneratorClass, GridPos, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	};
}