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

/**
 * Constructs the atom instances in OxygenSpawner blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void ABondSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

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

	//UE_LOG(LogTemp, Warning, TEXT("FinalMap key, value: %s, %s"), *pair.Key, *NewConectRow);

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
			//UE_LOG(LogTemp, Warning, TEXT("BondSpawner: AtomCoordMap[AtomIndex]: %s"), *AtomCoordMap[AtomIndex]);
			AtomCoordMap[AtomIndex].ParseIntoArray(ValueAtomCoordinates, DelimSpace, true);

			FVector ValueAtomCoordinatesVector = FVector(
				FCString::Atoi(*ValueAtomCoordinates[0]),
				FCString::Atoi(*ValueAtomCoordinates[1]), 
				FCString::Atoi(*ValueAtomCoordinates[2])) - CentroidCoordinate;

			// A bond's spawn point is located in the center of the bond cylinder.
			// Naturally, it follows that the bond should spawn in the bisect point between the 2 atoms.
			FVector BisectPoint = (KeyAtomCoordinatesVector + ValueAtomCoordinatesVector) / 2 - CentroidCoordinate;

			// Choose the value atom as the position that the top of the bond will align to, and subtract to align to center
			FVector VACVAlign = ValueAtomCoordinatesVector - BisectPoint;
			FVector VACVAlignN = VACVAlign.GetSafeNormal();

			// Normal vector corresponding to the top of the bond cylinder, bond centered @ origin
			FVector StartingVectorN = FVector(0, 0, 50).GetSafeNormal();

			// Rotation axis between the two vectors - used to determine rotation direction
			FVector Axis = FVector::CrossProduct(VACVAlignN, StartingVectorN);

			// Angle between the 2 vectors - for determining magnitude of rotation
			double Angle = FMath::Acos(FVector::DotProduct(VACVAlignN, StartingVectorN));
			double CosAngle = FMath::Cos(Angle);
			double SinAngle = FMath::Sin(Angle);

			// Find rotation matrix to align StartingVector to VACVAlign - Using Rodrigues rotation method
			FMatrix I4 = FMatrix::Identity; // 4 dimensional, remove translation to become 3D
			FMatrix I3 = I4.RemoveTranslation(); // 3D identity matrix - [[1, 0, 0], [0, 1, 0], [0, 0, 1]]

			/*K.M[0][0] = 0;
			K.M[0][1] = -StartingVectorN.Z;
			K.M[0][2] = StartingVectorN.Y;

			K.M[1][0] = StartingVectorN.Z;
			K.M[1][1] = 0;
			K.M[1][2] = -StartingVectorN.X;
			
			K.M[2][0] = -StartingVectorN.Y;
			K.M[2][1] = StartingVectorN.X;
			K.M[2][2] = 0;*/

			// Cross-product matrix K
			FMatrix K(
				FPlane(0.0f, -Axis.Z, Axis.Y, 0.0f),
				FPlane(Axis.Z, 0.0f, -Axis.X, 0.0f),
				FPlane(-Axis.Y, Axis.X, 0.0f, 0.0f),
				FPlane(0.0f, 0.0f, 0.0f, 1.0f)
			);

			// Rodrigues Rotation Matrix Formula
			FMatrix RotationMatrix = I3 +
				K * SinAngle +
				(K * K) * (1.0f - CosAngle);

		}
	}
	
	// Spawn all bonds
	// Should be a bond list
	// Or I could generate the instances as I go along
	// InstancedStaticMeshComponentBond->AddInstances()
	// 
	// the transform is simply the coordinates of where the cylinder should be placed.
	// InstancedStaticMeshComponentBond->AddInstance(TransformObject)

	
}

// Called every frame
void ABondSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

