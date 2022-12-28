#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NitrogenSpawner.generated.h"

UCLASS()
class PMUEDEVELOPMENTTWO_API ANitrogenSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANitrogenSpawner();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		TArray<FTransform> TransformsNitrogen;

private:
	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* InstancedStaticMeshComponentNitrogen;

};
