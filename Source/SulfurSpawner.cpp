#include "SulfurSpawner.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

// Sets default values
ASulfurSpawner::ASulfurSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InstancedStaticMeshComponentSulfur = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));

	//RootComponent = InstancedStaticMeshComponentSulfur;

	InstancedStaticMeshComponentSulfur->SetMobility(EComponentMobility::Movable);
	InstancedStaticMeshComponentSulfur->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentSulfur->SetGenerateOverlapEvents(false);
}

/**
 * Constructs the atom instances in SulfurSpawner blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void ASulfurSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ASulfurSpawner::BeginPlay()
{
	Super::BeginPlay();

	// file-specific variables
	FileData FD;
	FString PDBContent = FD.PDBContent;
	FVector CentroidCoordinate = FD.CentroidCoordinate;

	// variables specific to this spawner
	FString CurrentElement{ "S " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, false);
	TransformsSulfur.Empty(NumberOfAtoms);

	// generates data array of atoms to be spawned
	UToolsFunctionLibrary::CCESAtomGeneration(PDBContent, TransformsSulfur, CentroidCoordinate, CurrentElement, false);

	// spawns the actual static mesh
	InstancedStaticMeshComponentSulfur->AddInstances(TransformsSulfur, true);
	
}

// Called every frame
void ASulfurSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

