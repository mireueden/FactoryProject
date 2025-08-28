// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBasicDataAsset.h"
#include "ItemStorageStruct.generated.h"

class ItemBasicDataAsset;

struct ItemStorageStruct
{
	GENERATED_BODY()

	UPROPERTY()
	ItemBasicDataAsset* ItemData;

	UPROPERTY()
	int32 CurrentItemCount;

	UPROPERTY()
	int32 MaxStorageCount;
};

UCLASS()
class FACTORYPROJECT_API AItemStorageStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorageStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
