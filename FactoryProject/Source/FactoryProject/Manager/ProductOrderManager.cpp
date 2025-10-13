// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ProductOrderManager.h"

// Sets default values
AProductOrderManager::AProductOrderManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProductOrderManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProductOrderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProductOrderManager::ProductionCellSetting()
{
}

