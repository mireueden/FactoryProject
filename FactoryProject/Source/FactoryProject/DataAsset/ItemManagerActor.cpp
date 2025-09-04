// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ItemManagerActor.h"

AItemManagerActor::AItemManagerActor()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AItemManagerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AItemManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemManagerActor::OrderSpawn(int32 SelectedDataIndex, int32 OrderCount)
{
	if (!StorageList.IsValidIndex(SelectedDataIndex)) return;

	SelectedIndex = SelectedDataIndex;

	int32& CurrentCount = StorageList[SelectedIndex].CurrentItemCount;
	int32 MaxCount = StorageList[SelectedIndex].MaxStorageCount;

	int32 SpawnCount;

	// 기존 최대 개수보다 더 주문한 경우
	if (CurrentCount + OrderCount > MaxCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("You can't order more than MAXCount"));
		SpawnCount = MaxCount - CurrentCount;

		CurrentCount = MaxCount;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Order Count : %d"), OrderCount);
		SpawnCount = OrderCount;
		CurrentCount += OrderCount;
	}

	RemainingSpawnCount = SpawnCount;

	if (SpawnCount == 0)
		return;
	

	SpawnItem();

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AItemManagerActor::SpawnItem,
		8.0f,   // 체크 간격
		true    // 반복
	);
}

void AItemManagerActor::SpawnItem()
{
	if (RemainingSpawnCount <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		SelectedIndex = -1;
		RemainingSpawnCount = 0;
		return;
	}

	// Start 위치에 Item이 있는지 체크
	if (!StartConveyor->bStartTriggered)
	{
		FTransform SpawnTransform = StartConveyor->StartCollisionBox->GetComponentTransform();
		FVector SpawnLocation = SpawnTransform.GetLocation();
		SpawnLocation.Z += 40.0f;
		SpawnTransform.SetLocation(SpawnLocation);

		if (AItem* NewItem = GetWorld()->SpawnActor<AItem>(
			ItemClass,
			SpawnTransform
		))
		{
			NewItem->ItemData = StorageList[SelectedIndex].ItemData;
			//StartConveyor->RegisterItem(NewItem);
			NewItem->MeshComp->SetMaterial(0, NewItem->ItemData->ItemMaterial);
		}

		RemainingSpawnCount--;
	}

}

