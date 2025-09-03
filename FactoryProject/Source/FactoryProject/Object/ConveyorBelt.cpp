// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ConveyorBelt.h"

// Sets default values
AConveyorBelt::AConveyorBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->SetupAttachment(RootComponent);

	StartCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartCollisionBox"));
	StartCollisionBox->SetupAttachment(RootComponent);

	EndCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EndCollisionBox"));
	EndCollisionBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	
	if (SplineComp)
	{
		// 시작점
		FVector StartLocation = SplineComp->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		StartCollisionBox->SetWorldLocation(StartLocation);

		// 끝점
		int32 LastPointIndex = SplineComp->GetNumberOfSplinePoints() - 1;
		FVector EndLocation = SplineComp->GetLocationAtSplinePoint(LastPointIndex, ESplineCoordinateSpace::World);
		EndCollisionBox->SetWorldLocation(EndLocation);
	}
}

void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MoveItem();
}


void AConveyorBelt::SplineSetting(AItem * item)
{
	item->MoveSpeed = 100.f;
	item->SplineComp = SplineComp;
	item->ItemState = EItemState::Move;
}


void AConveyorBelt::MoveItem()
{
    if (ConveyorItemList.Num() == 0) return;

    for (int32 i = 0; i < ConveyorItemList.Num(); ++i)
    {
        AItem* Item = ConveyorItemList[i];
        if (!Item) continue;
        if (Item->ItemState != EItemState::Move) continue;

        float SplineLength = SplineComp->GetSplineLength();
        Item->SplineProgress += (Item->MoveSpeed * GetWorld()->GetDeltaSeconds()) / SplineLength;

        Item->SplineProgress = FMath::Clamp(Item->SplineProgress, 0.f, 1.f);

        FVector NewLocation = SplineComp->GetLocationAtDistanceAlongSpline(Item->SplineProgress * SplineLength, ESplineCoordinateSpace::World);
        FRotator NewRotation = SplineComp->GetRotationAtDistanceAlongSpline(Item->SplineProgress * SplineLength, ESplineCoordinateSpace::World);

        Item->SetActorLocationAndRotation(NewLocation, NewRotation);

        if (Item->SplineProgress >= 1.f)
        {
            Item->ItemState = EItemState::Stop;
            i--; 
        }
    }

	// for다 돈 이후 처리하기
	//if (PendingRemoveList.Num() > 0)
	//{
	//	for (AItem* Item : PendingRemoveList)
	//	{
	//		ConveyorItemList.Remove(Item);
	//		if (Item)
	//		{
	//			Item->ItemState = EItemState::Stop;
	//		}
	//	}
	//	PendingRemoveList.Empty();
	//}


}

void AConveyorBelt::RegisterItem(AItem* Item)
{
	ConveyorItemList.Add(Item);

	Item->ItemState = EItemState::Move;
}

void AConveyorBelt::UnRegisterItem(AItem* Item)
{
	ConveyorItemList.Remove(Item);
	
	Item->ItemState = EItemState::Stop;
}

