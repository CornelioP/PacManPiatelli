// Fill out your copyright notice in the Description page of Project Settings.


#include "FruitActor.h"

// Sets default values
AFruitActor::AFruitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFruitActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFruitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

