// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataAsset/ItemStorageStruct.h"
#include "ItemManagerObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FACTORYPROJECT_API AItemManagerObject : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItemStorageStruct> StorageList;
};
