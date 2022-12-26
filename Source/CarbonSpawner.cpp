#include "CarbonSpawner.h"
#include "ToolsFunctionLibrary.h"

// Sets default values
ACarbonSpawner::ACarbonSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InstancedStaticMeshComponentCarbon = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));

	//RootComponent = InstancedStaticMeshComponentCarbon;

	InstancedStaticMeshComponentCarbon->SetMobility(EComponentMobility::Movable);
	InstancedStaticMeshComponentCarbon->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentCarbon->SetGenerateOverlapEvents(false);
}

/**
 * Constructs the atom instances in CarbonSpawner blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void ACarbonSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ACarbonSpawner::BeginPlay()
{
	Super::BeginPlay();

	// file-specific variables - will be kept in a separate file called by all atom spawner Actors
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
	FVector CentroidCoordinate = UToolsFunctionLibrary::Centroid(PDBContent, false);

	// variables specific to this spawner
	FString CurrentElement{ "C " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, false);
	TransformsCarbon.Empty(NumberOfAtoms);

  // Generates data array of carbons to be spawned
	UToolsFunctionLibrary::CCESAtomGeneration(PDBContent, TransformsCarbon, CentroidCoordinate, CurrentElement, false);

	// spawns the actual static mesh	
	InstancedStaticMeshComponentCarbon->AddInstances(TransformsCarbon, true);

}

// Called every frame
void ACarbonSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

