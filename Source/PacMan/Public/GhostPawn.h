// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "Kismet/GameplayStatics.h"
#include "PacManPawn.h"
#include "GhostPawn.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AGhostPawn : public AMazePawn
{
	GENERATED_BODY()
public:
	AGhostPawn();

protected:
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnNodeReached() override;

private:
	UPROPERTY(VisibleAnywhere)
		class APacManPawn* Player;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		APacManPawn* GetPlayer() const;

	UFUNCTION()
		void SetSpeed(float Speed);

	UFUNCTION()
		virtual APacManNode* GetPlayerRelativeTarget();

	void SetGhostTarget();
};
