// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemBasicDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class FACTORYPROJECT_API UItemBasicDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* ItemMaterial;
};
