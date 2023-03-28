// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacManNode.generated.h"

UENUM()
enum EIsLegal { NotWalkable, Walkable };

UCLASS()
class PACMAN_API APacManNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APacManNode();

	//Proprieta per verificare se un nodo e o meno accessibile
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EIsLegal> EIsLegal = Walkable;

	// Posizione 2D del nodo
	UPROPERTY(EditAnywhere)
		FVector2D NodeGridPosition;

	// Posizione del nodo in coordinate 3D
	UPROPERTY(EditAnywhere)
		FVector NodeCoordinatesPosition;


	//Metodi

	// Setta le coordinate 2D del nodo
	void SetNodePosition(const double InX, const double InY);

	//Ritorna le coordinate 2D del nodo
	FVector2D GetNodePosition() const;

	//Ritorna le coordinate 3D del nodo
	FVector GetNodeCoordinates();

	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
