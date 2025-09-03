// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Object/Item.h"
#include "Object/ConveyorBelt.h"
#include "Components/BoxComponent.h"
#include "RobotArm.generated.h"

UCLASS()
class FACTORYPROJECT_API ARobotArm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotArm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* RobotArmComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RobotStandComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StandBoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CheckBox;

public:

	UPROPERTY()
	AConveyorBelt* CurrentConveyorBelt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowedClasses = "ItemBasicDataAsset"))
	UItemBasicDataAsset* CheckItemData;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AItem* GrabedItem;

	UFUNCTION(BlueprintCallable)
	void CheckItem(AItem* Item);

	void GrabItem(AItem* Item);


	UFUNCTION(BlueprintCallable)
	void SpreadItem(AItem* Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGrabItem = false;



};
