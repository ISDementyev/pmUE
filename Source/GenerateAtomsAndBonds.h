// See GPL-v3.0 LICENSE in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateAtomsAndBonds.generated.h"

UCLASS()
class PMUEINTEGRATION_API AGenerateAtomsAndBonds final : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerateAtomsAndBonds();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	/*UPROPERTY(Transient)
		TArray<FTransform> Transforms;*/

};
