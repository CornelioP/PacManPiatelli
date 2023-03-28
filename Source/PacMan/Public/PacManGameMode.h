// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeGenerator.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"

/**
 * 
 */
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

	APacManGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
