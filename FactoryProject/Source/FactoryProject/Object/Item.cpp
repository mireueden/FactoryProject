// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item.h"

#include "ConveyorBelt.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemState = EItemState::None;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

    if (ItemData && ItemData->ItemMaterial)
    {
        MeshComp->SetMaterial(0, ItemData->ItemMaterial);
    }
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


    if (ItemState != EItemState::Move || !ConveyorBelt->SplineComp) return;

    float SplineLength = ConveyorBelt->SplineComp->GetSplineLength();
    SplineProgress += (MoveSpeed * DeltaTime) / SplineLength;
    SplineProgress = FMath::Clamp(SplineProgress, 0.f, 1.f);

    FVector NewLocation = ConveyorBelt->SplineComp->GetLocationAtDistanceAlongSpline(SplineProgress * SplineLength, ESplineCoordinateSpace::World);
    FRotator NewRotation = ConveyorBelt->SplineComp->GetRotationAtDistanceAlongSpline(SplineProgress * SplineLength, ESplineCoordinateSpace::World);

    SetActorLocationAndRotation(NewLocation, NewRotation);

    if (SplineProgress >= 1.f)
    {
        ItemState = EItemState::Stop;
        ConveyorBelt->ItemArrive(this);
    }
}

