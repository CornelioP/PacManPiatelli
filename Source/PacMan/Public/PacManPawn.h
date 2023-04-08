// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "MazeGenerator.h"
#include "PacManPlayerController.h"
#include "PacManNode.h"
#include "GhostBaseNode.h"
#include "GhostPawn.h"
#include "PowerPoint.h"
#include "PacManPawn.generated.h"


UENUM()

enum EPacManStates { Normal,FrightenedP };

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


	UPROPERTY(EditAnywhere)
	TSubclassOf<AGhostBaseNode> GhostClass;

	AGhostBaseNode* GhostNode;

protected:
	virtual void BeginPlay() override;
	virtual void HandleMovement() override;
	virtual void SetTargetNode(APacManNode* Node) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
 
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "Variable" ,meta = (AllowPrivateAccess = true))
		float PointCounter;
	
	UPROPERTY(EditAnywhere)
		class ABlinky* Blinky;

	UPROPERTY(EditAnywhere)
		class AInky* Inky;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EPacManStates> PacManStates = Normal;
    
	void Eat(AGhostPawn* Ghost);

	FTimerHandle FrightnedTimer;

	FTimerHandle PointEaten;

	void FrightenedEnter();

	void FrightenedExit();

	void PointEatenExit();
};
