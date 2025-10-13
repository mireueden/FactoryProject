// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemProductionCell.h"

// Sets default values
AItemProductionCell::AItemProductionCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemProductionCell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemProductionCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemProductionCell::CraftingProduct()
{
}

