// See GPL-v3.0 LICENSE.MD in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AtomGeneratorOne.generated.h"

UCLASS()
class PMUEDEVELOPMENTNEW_API AAtomGeneratorOne : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAtomGeneratorOne();

	virtual void OnConstruction(const FTransform& Transform) override;

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
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
};
