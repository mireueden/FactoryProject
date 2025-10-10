// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Object/Item.h"
#include "ConveyorBelt.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemArrivedDelegate, AItem*, ArrivedItem);


UCLASS()
class FACTORYPROJECT_API AConveyorBelt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConveyorBelt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RobotArrivePoint;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* StartCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* EndCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	bool bStartTriggered;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	bool bEndTriggered;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	TArray<AItem*> ConveyorItemList;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnItemArrivedDelegate OnItemArrived;

	UFUNCTION(BlueprintCallable)
	void SplineSetting (AItem* item);

	UFUNCTION(BlueprintCallable)
	void ItemSetting (AItem* item);

	UFUNCTION(BlueprintCallable)
	void ItemArrive (AItem* item);










	void MoveItem();

	UFUNCTION(BlueprintCallable)
	void RegisterItem(AItem* item);

	UFUNCTION(BlueprintCallable)
	void UnRegisterItem(AItem* item);

};
