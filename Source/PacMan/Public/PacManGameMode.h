// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeGenerator.h"
#include "Blinky.h"
#include "Inky.h"
#include "PacManPawn.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"


//Game states

UENUM()
enum EStates { Chase, Scatter, Frightened };


UCLASS()
class PACMAN_API APacManGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	//Istanza di un oggetto MazeGenerator
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AMazeGenerator> MazeGeneratorClass;

	//Puntatore ad un oggetto MazeGenerator
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AMazeGenerator* GMaze;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (AllowPrivateAccess = true))
		float PointCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (AllowPrivateAccess = true))
		float LifeCounter;

	APacManGameMode();

	//State machine objects

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABlinky> BlinkyClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AInky> InkyClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APacManPawn> PacManClass;

	UPROPERTY(EditAnywhere)
		 ABlinky* Blinky;

	UPROPERTY(EditAnywhere)
		 AInky* Inky;

	UPROPERTY(EditAnywhere)
		 APacManPawn* PacMan;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EStates> EStates;

	//Int to save last state when entered Frightened Mode

	UPROPERTY(EditAnywhere)
	int LastState;

    //Counter to see how many times Scatter state has been activated

	UPROPERTY(EditAnywhere)
		int Scatter_Counter;

	//Counter to see how many times Chase state has been activated
	
	UPROPERTY(EditAnywhere)
		int Chase_Counter;

	//Timers

	FTimerHandle FrightnedTimer;
	
	FTimerHandle ChaseTimer;

	FTimerHandle ScatterTimer;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//State Functions

	void EnterFrightenedMode();

	void FrightenedExit();

	void EnterChaseMode();

	void ChaseExit();

	void EnterScatterMode();

	void ScatterExit();
};
