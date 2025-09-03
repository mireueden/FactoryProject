// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemState = EItemState::None;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


    if (ItemState != EItemState::Move || !SplineComp) return;

    float SplineLength = SplineComp->GetSplineLength();
    SplineProgress += (MoveSpeed * DeltaTime) / SplineLength;
    SplineProgress = FMath::Clamp(SplineProgress, 0.f, 1.f);

    FVector NewLocation = SplineComp->GetLocationAtDistanceAlongSpline(SplineProgress * SplineLength, ESplineCoordinateSpace::World);
    FRotator NewRotation = SplineComp->GetRotationAtDistanceAlongSpline(SplineProgress * SplineLength, ESplineCoordinateSpace::World);

    SetActorLocationAndRotation(NewLocation, NewRotation);

    if (SplineProgress >= 1.f)
    {
        ItemState = EItemState::Stop;
    }
}

