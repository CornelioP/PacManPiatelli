// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacManPawn.h"

void AInky::OnNodeReached()
{
    if (Player->PointCounter >= 30.0)
    {
        Super::OnNodeReached();
    }
}
