// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "NavArea_AvoidCell.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UNavArea_AvoidCell : public UNavArea
{
	GENERATED_BODY()
	
	UNavArea_AvoidCell()
	{
		DefaultCost = 100.f;
		FixedAreaEnteringCost = 100.f;
		DrawColor = FColor::Red; // 디버그용 색상
	}
};
