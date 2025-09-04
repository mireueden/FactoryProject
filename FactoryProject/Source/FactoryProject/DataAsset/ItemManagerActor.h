// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAsset/ItemStorageStruct.h"
#include "Object/ConveyorBelt.h"
#include "ItemManagerActor.generated.h"

UCLASS(Blueprintable,BlueprintType)
class FACTORYPROJECT_API AItemManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItemStorageStruct> StorageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AConveyorBelt* StartConveyor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AItem> ItemClass;


	UFUNCTION()
	void OrderSpawn(int32 SelectedIndex, int32 OrderCount);

	int32 SelectedIndex;
	int32 RemainingSpawnCount = 0;
	FTimerHandle SpawnTimerHandle;

	void SpawnItem();
};
