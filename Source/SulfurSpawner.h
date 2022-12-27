#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SulfurSpawner.generated.h"

UCLASS()
class PMUEDEVELOPMENTTWO_API ASulfurSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASulfurSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		TArray<FTransform> TransformsSulfur;

private:
	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* InstancedStaticMeshComponentSulfur;
};
