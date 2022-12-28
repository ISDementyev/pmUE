#include "OxygenSpawner.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

// Sets default values
AOxygenSpawner::AOxygenSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InstancedStaticMeshComponentOxygen = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));

	//RootComponent = InstancedStaticMeshComponentOxygen;

	InstancedStaticMeshComponentOxygen->SetMobility(EComponentMobility::Movable);
	InstancedStaticMeshComponentOxygen->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentOxygen->SetGenerateOverlapEvents(false);
}

/**
 * Constructs the atom instances in OxygenSpawner blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void AOxygenSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void AOxygenSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// file-specific variables
	FileData FD;
	FString PDBContent = FD.PDBContent;
	FVector CentroidCoordinate = FD.CentroidCoordinate;

	// variables specific to this spawner
	FString CurrentElement{ "O " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, false);
	TransformsOxygen.Empty(NumberOfAtoms);

	// generates data array of atoms to be spawned
	UToolsFunctionLibrary::CCESAtomGeneration(PDBContent, TransformsOxygen, CentroidCoordinate, CurrentElement, false);

	// spawns the actual static mesh
	InstancedStaticMeshComponentOxygen->AddInstances(TransformsOxygen, true);
}

// Called every frame
void AOxygenSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

