#include "HydrogenSpawner.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

// Sets default values
AHydrogenSpawner::AHydrogenSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InstancedStaticMeshComponentHydrogen = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));

	//RootComponent = InstancedStaticMeshComponentHydrogen;

	InstancedStaticMeshComponentHydrogen->SetMobility(EComponentMobility::Movable);
	InstancedStaticMeshComponentHydrogen->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentHydrogen->SetGenerateOverlapEvents(false);
}

/**
 * Constructs the atom instances in HydrogenSpawner blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void AHydrogenSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void AHydrogenSpawner::BeginPlay()
{
	Super::BeginPlay();

	// file-specific variables
	FileData FD;
	FString PDBContent = FD.PDBContent;
	FVector CentroidCoordinate = FD.CentroidCoordinate;

	// variables specific to this spawner
	FString CurrentElement{ "H " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, false);
	TransformsHydrogen.Empty(NumberOfAtoms);

	// generates data array of atoms to be spawned
	UToolsFunctionLibrary::CCESAtomGeneration(PDBContent, TransformsHydrogen, CentroidCoordinate, CurrentElement, false);

	// spawns the actual static mesh
	InstancedStaticMeshComponentHydrogen->AddInstances(TransformsHydrogen, true);
	
}

// Called every frame
void AHydrogenSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

