// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BondSpawner.generated.h"

UCLASS()
class PMUEDEVELOPMENTTWO_API ABondSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABondSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		TArray<FTransform> TransformsBonds;

private:
	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* InstancedStaticMeshComponentBond;

};
