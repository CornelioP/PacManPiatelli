// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacManNode.h"
//#include "PointActor.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class PACMAN_API AMazeGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMazeGenerator();

	//Array di puntatori a PacManNode
	UPROPERTY(VisibleAnywhere)
		TArray<APacManNode*> Node;

	//Dimensione di un nodo
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float NodeSize;

private:


	//Tipi di Nodo Possibili 

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> WallNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> PowerNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> PointNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> LabyrinthNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> NullNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> CharacterNode;

	UPROPERTY(EditDefaultsOnly, Category = "Ghosts")
		TSubclassOf<APacManNode> BlinkyNode;

	UPROPERTY(EditDefaultsOnly, Category = "Ghosts")
		TSubclassOf<APacManNode> PinkyNode;

	UPROPERTY(EditDefaultsOnly, Category = "Ghosts")
		TSubclassOf<APacManNode> InkyNode;

	UPROPERTY(EditDefaultsOnly, Category = "Ghosts")
		TSubclassOf<APacManNode> ClydeNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> GhostRespawnNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> TeleportNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> GhostAreaNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> InvisibleWallNode;

	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<APacManNode> GhostExitNode;

	UPROPERTY(EditDefaultsOnly)
		FVector SpawnOffset;

	//UPROPERTY(EditDefaultsOnly, Category = "Point")
		//TSubclassOf<APointActor> Point;

	//UPROPERTY(EditDefaultsOnly, Category = "Point")
		//TSubclassOf<APointActor> PowerPoint;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Metodi

	//Data la posizione 2D ritorna un Node
	TMap<FVector2D, APacManNode*> TileMap;

	TMap<FVector2D, APacManNode*> GetTileMAp();

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<APacManNode*>& GetTileArray();

	// return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY);

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location);

	//Funzione che genera il labirinto
	void GenerateMaze();

	//Function to spawn an actor given the CharId 
	APacManNode* SpawnNodeActorById(char CharId, FVector Position) const;
	//Function to check if a node is walkable
	static bool IsNodeValidForWalk(class APacManNode* Node);

	APacManNode* GetNextNode(const FVector2D StartCoords, FVector InputDir);

	FVector2D GetTwoDOfVector(FVector DDDVector);

	//Function that return the array of walkable near node

	TArray<APacManNode*> NearPossibleNode(APacManNode* NodeInp);

	//Function that return the nearst possible node given target destination

	APacManNode* ShortestNodeToTarget(APacManNode* NodeInp, APacManNode* TargetNode);
};