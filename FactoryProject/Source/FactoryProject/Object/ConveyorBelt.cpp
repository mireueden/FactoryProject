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

// Called every frame
void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveItem();
}

void AConveyorBelt::MoveItem()
{
    if (ConveyorItemList.Num() == 0) return;

    for (int32 i = 0; i < ConveyorItemList.Num(); ++i)
    {
        AItem* Item = ConveyorItemList[i];
        if (!Item) continue;

        // Item의 상태가 Move일 때만 이동
        if (Item->ItemState != EItemState::Move) continue;

        // Spline 진행도 계산
        float SplineLength = SplineComp->GetSplineLength();
        Item->SplineProgress += (Item->MoveSpeed * GetWorld()->GetDeltaSeconds()) / SplineLength;

        // Clamp: 0 ~ 1 범위
        Item->SplineProgress = FMath::Clamp(Item->SplineProgress, 0.f, 1.f);

        // Spline 위치 & 회전 적용
        FVector NewLocation = SplineComp->GetLocationAtDistanceAlongSpline(Item->SplineProgress * SplineLength, ESplineCoordinateSpace::World);
        FRotator NewRotation = SplineComp->GetRotationAtDistanceAlongSpline(Item->SplineProgress * SplineLength, ESplineCoordinateSpace::World);

        Item->SetActorLocationAndRotation(NewLocation, NewRotation);

        // EndCollision 체크 (Spline 끝에 도달하면 Stop 처리)
        if (Item->SplineProgress >= 1.f)
        {
            Item->ItemState = EItemState::Stop;
            i--; // Remove 했으니 인덱스 보정
        }
    }
	
}

void AConveyorBelt::RegisterItem(AItem* item)
{
	ConveyorItemList.Add(item);

}

void AConveyorBelt::UnRegisterItem(AItem* item)
{
	ConveyorItemList.Remove(item);
}

