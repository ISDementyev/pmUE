// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OxygenSpawner.generated.h"

UCLASS()
class PMUEDEVELOPMENTTWO_API AOxygenSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOxygenSpawner();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		TArray<FTransform> TransformsOxygen;

private:
	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* InstancedStaticMeshComponentOxygen;

};
