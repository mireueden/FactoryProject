// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Object/Item.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Object/RobotArm.h"
#include "Object/CellRobotArm.h"

#include "NavModifierComponent.h"
#include "AI/NavArea_AvoidCell.h"

#include "ItemProductionCell.generated.h"

UENUM(BlueprintType)
enum class ECellProgressState :uint8
{
	None         UMETA(DisplayName = "None"),          
	Empty        UMETA(DisplayName = "Empty"),          // 아무 것도 없음, 대기 가능
	Reserved     UMETA(DisplayName = "Reserved"),       // Robot이 이동 예정, 예약
	InProgress   UMETA(DisplayName = "InProgress"),     // 작업 중
	Completed    UMETA(DisplayName = "Completed"),		// 작업 완료 및 대기
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCellStateChanged);

class ADeliveryRobot;

UCLASS()
class FACTORYPROJECT_API AItemProductionCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemProductionCell();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MainStaticComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftStandMatComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightStandMatComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RobotFloorComp;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* DetectArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNavModifierComponent* NavModifier;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UItemBasicDataAsset* ProductProcessData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ADeliveryRobot* ProcessRobot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ADeliveryRobot* BeforeProcessRobot;

	UPROPERTY(BlueprintAssignable)
	FOnCellStateChanged OnCellStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ECellProgressState CurrentState = ECellProgressState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Arm")
	TSubclassOf<AActor> ArmActorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Robot Arm")
	TArray<ACellRobotArm*> CellRobotArmList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Func")
	FRotator FloorRotateValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Func")
	FVector FloorLocateValue;

	FRotator InitialRot;
	FVector InitialLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Func")
	float MoveSpeed = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Func")
	float RotateSpeed = 2.0f;

	UPROPERTY()
	bool bFloor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Func")
	bool bIsMoving = false;

	UFUNCTION(CallInEditor, Category = "Floor Func")
	void FloorMove();
	



	
	UFUNCTION(CallInEditor, Category = "Robot Arm")
	void SpawnAndAttachArm(UStaticMeshComponent* ParentComp, const FVector& Offset, const FRotator& RotOffset);

	
	UFUNCTION()
	void OnRobotEnterCell(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnRobotExitCell(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void CellStateChanged(ECellProgressState NewState);


	UFUNCTION(CallInEditor, Category = "Production")
	void ProductionProcess();
	
	UFUNCTION(CallInEditor, Category = "Production")
	void CraftingProduct();

	UFUNCTION(Category = "Production")
	void ReadyToCraftingProduct();

};
