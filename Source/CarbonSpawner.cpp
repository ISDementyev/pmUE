// Fill out your copyright notice in the Description page of Project Settings.


#include "CarbonSpawner.h"
#include "ToolsFunctionLibrary.h"

// Sets default values
ACarbonSpawner::ACarbonSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACarbonSpawner::BeginPlay()
{
	Super::BeginPlay();

	// "global" variables - should be kept in a separate file called by all spawner Actors
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
	FVector CentroidCoordinate = UToolsFunctionLibrary::Centroid(PDBContent, false);
	TMap<int32, FString> IndicesAndAtomNames = UToolsFunctionLibrary::AtomNameAndIndex(PDBContent, false);

	// variables specific to this spawner
	FString CurrentElement{ "C " };
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtomsSingleElement(PDBContent, CurrentElement, false);
	Transforms.Empty(NumberOfAtoms);

}

// Called every frame
void ACarbonSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

