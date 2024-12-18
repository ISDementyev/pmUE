// Fill out your copyright notice in the Description page of Project Settings.


#include "CarbonSpawner.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

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

	// file-specific variables - should be kept in a separate file called by all atom spawner Actors
	FileData FD;	
	FString PDBContent = FD.PDBContent;
	FVector CentroidCoordinate = FD.CentroidCoordinate;
	bool AtomOnly = FD.AtomOnly;

	// variables specific to this spawner
	FString CurrentElement{ "C " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, AtomOnly);
	UE_LOG(LogTemp, Warning, TEXT("CarbonSpawner: NumberOfAtoms: %d"), NumberOfAtoms);

	TArray<int32> CarbonSerials = UToolsFunctionLibrary::ElementIndices(PDBContent, CurrentElement, AtomOnly);
	TransformsCarbon.Empty(NumberOfAtoms);

	// generates data array of atoms to be spawned
	UToolsFunctionLibrary::CCESAtomGeneration(PDBContent, TransformsCarbon, CentroidCoordinate, CurrentElement, AtomOnly);

	// spawns the actual static mesh
	InstancedStaticMeshComponentCarbon->AddInstances(TransformsCarbon, true);
}

// Called every frame
void ACarbonSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

