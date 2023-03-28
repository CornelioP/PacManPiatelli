// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

constexpr int MapSizeX = 35;
// la y contiene 28 elementi (+ il carattere di fine stringa)
constexpr int MapSizeY = 29;

// # - Wall
// B - Big Point
// N - Empty
// o - Point
//     Phantom:
//     - 1 - Blinky
//     - 2 - Pinky
//     - 3 - Inky
//     - 4 - Clyde
//    
// C - Cherry
// R - Ghost Respawn
// N - Ghost Area
// I - Invisible Not Walkable
// E - Ghost Exit

static char Map[MapSizeX][MapSizeY] = {
	"############################",
	"############################",
	"############################",
	"#oooooooooooooooooooooooooo#",
	"#o##########o##o##########o#",
	"#o#######oooo##oooo#######o#",
	"#oooooo##o########o##oooooo#",
	"###o##o##o########o##o##o###",
	"#Boo##ooooooo  ooooooo##ooB#",
	"#o####o#####o##o#####o####o#",
	"#o####o#####o##o#####o####o#",
	"#oooooooooooo##oooooooooooo#",
	"######o## ######## ##o######",
	"######o## ######## ##o######",
	"######o## 12 RE 34 ##o######",
	"######o## ###II### ##o######",
	"######o## #IIIIII# ##o######",
	"T     o   #IIIIII#   o     T",
	"######o## #IIIIII# ##o######",
	"######o## ######## ##o######",
	"######o##          ##o######",
	"######o##### ## #####o######",
	"######o##### ## #####o######",
	"#oooooo##oooo##oooo##oooooo#",
	"#o####o##o########o##o####o#",
	"#o####o##o########o##o####o#",
	"#oooooooooooooooooooooooooo#",
	"#o####o#####o##o#####o####o#",
	"#B####o#####o##o#####o####B#",
	"#o####o#####o##o#####o####o#",
	"#oooooooooooo##oooooooooooo#",
	"############################",
	"############################",
	"############################",
};

// Sets default values
AMazeGenerator::AMazeGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NodeSize = 100.0f;
	SpawnOffset = FVector(NodeSize);

}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateMaze();

}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMazeGenerator::GenerateMaze()
{

	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY - 1; y++)
		{
			const char MapTile = Map[x][y];

			FVector OffsetVector(x * SpawnOffset.X, y * SpawnOffset.Y, 0);
			// imposto le coordinate spaziali per la funzione di spawn della tile
			const FVector CurrentSpawnPosition = GetActorLocation() + OffsetVector;
			// questa funzione spawna una nuova tile
			const auto SpawnedNode = SpawnNodeActorById(MapTile, CurrentSpawnPosition);
			// assegna le coordinate di griglia alla tile
			SpawnedNode->NodeGridPosition = (FVector2D(x, y));
			// assegna le coordinate spaziali alla tile
			SpawnedNode->NodeCoordinatesPosition = CurrentSpawnPosition;
			// aggiungo alle strutture dati il riferimento alla tile creata
			Node.Add(SpawnedNode);
			TileMap.Add(FVector2D(x, y), SpawnedNode);
		}
	}
}

APacManNode* AMazeGenerator::SpawnNodeActorById(char CharId, FVector Position) const
{
	APacManNode* Node_tmp;
	TSubclassOf<APacManNode> ClassToSpawn = APacManNode::StaticClass();


	switch (CharId)
	{
	case '#': ClassToSpawn = WallNode; break;
	case 'B': ClassToSpawn = PowerNode; break;
	case '1': ClassToSpawn = BlinkyNode; break;
	case '2': ClassToSpawn = PinkyNode; break;
	case '3': ClassToSpawn = InkyNode; break;
	case '4': ClassToSpawn = ClydeNode; break;
	case 'R': ClassToSpawn = GhostRespawnNode; break;
	case 'T': ClassToSpawn = TeleportNode; break;
	case 'N': ClassToSpawn = GhostAreaNode; break;
	case 'I': ClassToSpawn = InvisibleWallNode; break;
	case 'E': ClassToSpawn = GhostExitNode; break;
	case 'o': ClassToSpawn = PointNode; break;
	default: ClassToSpawn = LabyrinthNode; break;
	}

	//Check to verify if it should spawn also a point actor

	Node_tmp = GetWorld()->SpawnActor<APacManNode>(ClassToSpawn, Position, FRotator::ZeroRotator);

	if (ClassToSpawn == PointNode)
	{
		//GetWorld()->SpawnActor<APointActor>(Point, Position, FRotator::ZeroRotator);
	}
	if (ClassToSpawn == PowerNode)
	{
		//GetWorld()->SpawnActor<APointActor>(PowerPoint, Position, FRotator::ZeroRotator);
	}


	return Node_tmp;
}


TMap<FVector2D, APacManNode*> AMazeGenerator::GetTileMAp()
{
	return TileMap;
}

FVector2D AMazeGenerator::GetPosition(const FHitResult& Hit)
{
	return Cast<APacManNode>(Hit.GetActor())->GetNodePosition();
}

TArray<APacManNode*>& AMazeGenerator::GetTileArray()
{
	return Node;
}

FVector AMazeGenerator::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	return  NodeSize * FVector(InX, InY, 0);
}

FVector2D AMazeGenerator::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = floor(Location[0] / (NodeSize));
	double y = floor(Location[1] / (NodeSize));
	return FVector2D(x, y);

}

bool AMazeGenerator::IsNodeValidForWalk(APacManNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EIsLegal == NotWalkable) return false;

	return true;
}

APacManNode* AMazeGenerator::GetNextNode(const FVector2D StartCoords, FVector InputDir)
{

	const float RequestedX = StartCoords.X + InputDir.X;
	const float RequestedY = StartCoords.Y + InputDir.Y;

	//Return a number in a given interval 

	const float ClampedX = FMath::Clamp(RequestedX, 0.f, MapSizeX - 1);
	const float ClampedY = FMath::Clamp(RequestedY, 0.f, MapSizeY - 2);

	APacManNode* PossibleNode = GetTileMAp()[FVector2D(ClampedX, ClampedY)];

	if (RequestedX > ClampedX || RequestedX < 0)
	{
		//X overflow
		PossibleNode = nullptr;
	}
	if (RequestedY > ClampedY || RequestedY < 0)
	{
		//Y overflow
		PossibleNode = nullptr;
	}
	return PossibleNode;
}

FVector2D AMazeGenerator::GetTwoDOfVector(FVector DDDVector)
{
	return FVector2D(DDDVector.X, DDDVector.Y);;
}



//Functions to handle Ghost movement


TArray<APacManNode*> AMazeGenerator::NearPossibleNode(APacManNode* NodeInp)
{
	TArray<APacManNode*> PossibleNodes;

	if (IsNodeValidForWalk(GetNextNode(NodeInp->GetNodePosition(), FVector::ForwardVector)))
	{
		PossibleNodes.Add(GetNextNode(NodeInp->GetNodePosition(), FVector::ForwardVector));
	}
	else if (IsNodeValidForWalk(GetNextNode(NodeInp->GetNodePosition(), FVector::RightVector)))
	{
		PossibleNodes.Add(GetNextNode(NodeInp->GetNodePosition(), FVector::RightVector));
	}
	else if (IsNodeValidForWalk(GetNextNode(NodeInp->GetNodePosition(), -FVector::ForwardVector)))
	{
		PossibleNodes.Add(GetNextNode(NodeInp->GetNodePosition(), -FVector::ForwardVector));
	}
	else if (IsNodeValidForWalk(GetNextNode(NodeInp->GetNodePosition(), FVector::LeftVector)))
	{
		PossibleNodes.Add(GetNextNode(NodeInp->GetNodePosition(), FVector::LeftVector));
	}

	return PossibleNodes;
}

APacManNode* AMazeGenerator::ShortestNodeToTarget(APacManNode* NodeInp, APacManNode* TargetNode)
{
	TArray<APacManNode*> PossibleNodes = NearPossibleNode(NodeInp);
	APacManNode* ShortestNode = nullptr;
	float Distance = 100000000000000.0;
	for (int i = 0; i < PossibleNodes.Num(); i++)
	{
		float Distance_tmp = FVector2D::Distance(PossibleNodes[i]->GetNodePosition(), TargetNode->GetNodePosition());

		if (Distance_tmp < Distance)
		{
			Distance = Distance_tmp;
			ShortestNode = PossibleNodes[i];
		}


	}

	return ShortestNode;

}