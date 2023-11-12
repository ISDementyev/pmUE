// Fill out your copyright notice in the Description page of Project Settings.


#include "BondSpawner.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

// Sets default values
ABondSpawner::ABondSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABondSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	const TCHAR* Delim = TEXT(",");
	const TCHAR* DelimSpace = TEXT(" ");

	// file-specific variables - should be kept in a separate file called by all atom spawner Actors
	FileData FD;
	FString PDBContent = FD.PDBContent;
	FVector CentroidCoordinate = FD.CentroidCoordinate;
	bool AtomOnly = FD.AtomOnly;
	int32 NumberOfAtoms = FD.NAtoms;

	// variables specific to this spawner
	TMap<int32, FString> AtomCoordMap = UToolsFunctionLibrary::AtomIndexAndCoordMap(PDBContent, NumberOfAtoms, AtomOnly);
	TMap<FString, FString> ConectMap = UToolsFunctionLibrary::ConectInfo(PDBContent);

	for (const TPair<FString, FString>& Pair : ConectMap)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ConectMap key, value: %s, %s"), *Pair.Key, *Pair.Value);
		int32 Index = FCString::Atoi(*Pair.Key) - 1; // The index of the current atom as it corresponds to the AtomCoordMap
		TArray<FString> BondedArray; // The array containing the atom indices that Index is bonded to in the molecule
		Pair.Value.ParseIntoArray(BondedArray, Delim, true);
		TArray<FString> KeyAtomCoordinates; // The pre-fvector tarray of the key's atom coordinates
		AtomCoordMap[Index].ParseIntoArray(KeyAtomCoordinates, DelimSpace, true); // Puts coordinates into KAC array
		
		// The FVector equivalent of the KAC Array
		FVector KeyAtomCoordinatesVector = FVector(FCString::Atoi(*KeyAtomCoordinates[0]), 
			FCString::Atoi(*KeyAtomCoordinates[1]), FCString::Atoi(*KeyAtomCoordinates[2])) - CentroidCoordinate;

		for (const FString OriginalIndex : BondedArray)
		{
			int32 AtomIndex = FCString::Atoi(*OriginalIndex) - 1;
			TArray<FString> ValueAtomCoordinates;
			AtomCoordMap[AtomIndex].ParseIntoArray(ValueAtomCoordinates, DelimSpace, true);

			FVector ValueAtomCoordinatesVector = FVector(FCString::Atoi(*ValueAtomCoordinates[0]),
				FCString::Atoi(*ValueAtomCoordinates[1]), FCString::Atoi(*ValueAtomCoordinates[2])) - CentroidCoordinate;

			FVector BisectPoint = (KeyAtomCoordinatesVector + ValueAtomCoordinatesVector) / 2 - CentroidCoordinate; // where the bond will spawn
		}
	}
	
}

// Called every frame
void ABondSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

