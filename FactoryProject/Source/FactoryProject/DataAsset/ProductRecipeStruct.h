// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "ProductRecipeStruct.generated.h"


USTRUCT(BlueprintType)
struct FProductRecipeStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product", meta = (AllowedClasses = "ItemBasicDataAsset"))
	UItemBasicDataAsset* ItemData;
	// ItemIndex와 ItemName 가져와서 사용하기

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	int32 RequiredItem;


};
