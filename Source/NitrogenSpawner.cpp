#include "NitrogenSpawner.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

// Sets default values
ANitrogenSpawner::ANitrogenSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InstancedStaticMeshComponentNitrogen = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));

	//RootComponent = InstancedStaticMeshComponentNitrogen;

	InstancedStaticMeshComponentNitrogen->SetMobility(EComponentMobility::Movable);
	InstancedStaticMeshComponentNitrogen->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentNitrogen->SetGenerateOverlapEvents(false);
}

/**
 * Constructs the atom instances in NitrogenSpawner blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void ANitrogenSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ANitrogenSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// file-specific variables
	FileData FD;
	FString PDBContent = FD.PDBContent;
	FVector CentroidCoordinate = FD.CentroidCoordinate;
	bool AtomOnly = FD.AtomOnly;

	// variables specific to this spawner
	FString CurrentElement{ "N " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, AtomOnly);
	TransformsNitrogen.Empty(NumberOfAtoms);

	// generates data array of atoms to be spawned
	UToolsFunctionLibrary::CCESAtomGeneration(PDBContent, TransformsNitrogen, CentroidCoordinate, CurrentElement, AtomOnly);

	// spawns the actual static mesh
	InstancedStaticMeshComponentNitrogen->AddInstances(TransformsNitrogen, true);
}

// Called every frame
void ANitrogenSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
