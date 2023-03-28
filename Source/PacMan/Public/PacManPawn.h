// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "MazeGenerator.h"
#include "PacManPlayerController.h"
#include "PacManNode.h"
#include "PacManPawn.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APacManPawn : public AMazePawn
{
	GENERATED_BODY()

	APacManPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetVerticalInput(float AxisValue) override;
	virtual void SetHorizontalInput(float AxisValue) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void HandleMovement() override;
	virtual void SetTargetNode(APacManNode* Node) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
