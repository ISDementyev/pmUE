// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarbonSpawner.generated.h"

UCLASS()
class PMUEDEVELOPMENTTWO_API ACarbonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarbonSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient)
		TArray<FTransform> Transforms;

private:
	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* InstancedStaticMeshComponentCarbon;
};
