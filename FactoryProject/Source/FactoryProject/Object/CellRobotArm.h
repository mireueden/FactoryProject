// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

#include "CellRobotArm.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestProductProcess, ACellRobotArm*, Arm);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequestProductProcessDone);

UCLASS()
class FACTORYPROJECT_API ACellRobotArm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellRobotArm();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RobotStandComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* RobotArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> ArmAnimClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "RobotArm")
	AItemProductionCell* GetOwnerCell();

	UPROPERTY(BlueprintAssignable)
	FOnRequestProductProcessDone OnRequestProductProcessDone;

	UFUNCTION(BlueprintCallable, Category = "RobotArm")
	void ProductRequest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotArm")
	bool bIsGrabedMesh = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotArm")
	bool bIsProcess = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotArm")
	float RobotArmRotateValue = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotArm")
	FRotator ItemRotation = FRotator(0.f, 0.f, 0.f);
};
