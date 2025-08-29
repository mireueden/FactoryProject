// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "ItemStorageStruct.generated.h"


USTRUCT(BlueprintType)
struct FItemStorageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowedClasses = "ItemBasicDataAsset"))
	UItemBasicDataAsset* ItemData;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 CurrentItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStorageCount;
};
