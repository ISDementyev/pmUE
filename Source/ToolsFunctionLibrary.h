// See GPL-v3.0 LICENSE in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolsFunctionLibrary.generated.h"

/**
 * Tools blueprint function library
 */
UCLASS()
class PMUEDEVELOPMENTTWO_API UToolsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		/**
		 * Converts file content into string
		 * @param Filename Name of the file
		 * @return The file's content as an FString
		*/
		static FString ConvFileToString(FString Filename);

	UFUNCTION(BlueprintCallable)
		/**
		 * Counts the total number of atoms in the pdb file
		 * @param LoadedString String loaded from pdb file
		 * @param bOnlyAtom Boolean that, if true (by default), does not consider heteroatoms (HETATM rows in pdb file)
		 * @return An integer number of atoms in pdb file
		*/
		static int32 NumberOfAtoms(FString& LoadedString, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Counts the number of atoms in the pdb file, specific to the spawner's element
		 * @param LoadedString String loaded from pdb file
		 * @param CurrentElement The element corresponding to the spawner
		 * @param bOnlyAtom Boolean that, if true (by default), does not consider heteroatoms (HETATM rows in pdb file)
		 * @return An integer number of atoms in pdb file
		*/
		static int32 NumberOfAtomsSingleElement(FString& LoadedString, FString& CurrentElement, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Returns set of all unique elements found in the pdb - useful for blueprint generation/modification
		 * @param LoadedString String loaded from pdb file
		 * @return Set of all unique elements (not including hetatms)
		*/
		static TSet<FString> UniqueElements(FString& LoadedString, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Getter function for a 3-vector corresponding to the input atom's color
		 * @param ChemicalSymbol The atom's chemical symbol, as a string
		 * @return RGB vector corresponding to the input atom
		*/
		static FVector GetElementColourRGB(FString& ChemicalSymbol);

	UFUNCTION(BlueprintCallable)
		/**
		 * Returns van der Waals radius of input atom, based on the averages of Tables 1, 2 and 7 in "Van der Waals Radii of Elements" (Batsanov, 2001).
		 * @param ChemicalSymbol The atom's chemical symbol
		 * @return The van der Waals radius of the atom (in nm)
		*/
		static float GetVDWRadius(FString ChemicalSymbol, TMap<FString, float>& VDWRadiiMap);

	UFUNCTION(BlueprintCallable)
		/**
		 * Reads and returns coordinates from PDB
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @param Transforms The Coordinate array
		 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
		*/
		static void GetCoordinates(FString& LoadedString, TArray<FTransform>& Transforms, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Helper function - Returns the centroid of the PDB, based on atom positions
		 * @param LoadedString The string contents of the PDB file
		 * @param bOnlyAtom
		 * @return PDB file's centroid coordinate
		*/
		static FVector Centroid(FString& LoadedString, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Reads and spawns centroid-corrected coordinates from PDB
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @param Transforms The coordinate array
		 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards heteroatoms (e.g. "HETATM" and "ANISOU" rows, etc.)
		 * @param Centroid Centroid calculated using UToolsFunctionLibrary::Centroid
		*/
		static void CentroidCorrected(FString& LoadedString, TArray<FTransform>& Transforms, FVector& CentroidCoord, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Centroid-Corrected Element-Specific Atom Generator: Reads and returns centroid-corrected coordinates from PDB
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @param Transforms The coordinate array
		 * @param CentroidCoord Centroid of the pdb file, can be calculated using UToolsFunctionLibrary::Centroid
		 * @param Element The atom's element
		 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards others (e.g. "HETATM" and "ANISOU" rows, etc.)
		*/
		static void CCESAtomGeneration(FString& LoadedString, TArray<FTransform>& Transforms, FVector& CentroidCoord, FString& Element, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Returns indices of atoms from the PDB file string contents
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @param NumberOfAtoms Number of atoms in the PDB 
		 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
		 * @return Map (dictionary) of atom names and their indices from the pdb - format is (Index: AtomName)
		*/
		static TMap<int32, FString> AtomIndexAndCoordMap(FString& LoadedString, int32 NumberOfAtoms, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Updates atom position and size
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @param CentroidCoord Centroid of the pdb file, can be calculated using UToolsFunctionLibrary::Centroid
		 * @param ISMC InstancedStaticMeshComponent of the actor
		 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards heteroatoms (e.g. "HETATM" and "ANISOU" rows, etc.)
		*/
		static void UpdateCoordinates(FString& LoadedString, FVector& CentroidCoord, UInstancedStaticMeshComponent* ISMC, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Returns UE Array containing indices of elements
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @param AtomName Name of the info whose connectivity info we need
		 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards heteroatoms (e.g. "HETATM" and "ANISOU" rows, etc.)
		 * @return UE vector containing indices of atoms that AtomIndex's atom is connected to
		*/
		static TArray<int32> ElementIndices(FString& LoadedString, FString ElementName, bool bOnlyAtom = true);

	UFUNCTION(BlueprintCallable)
		/**
		 * Helper function, retrieves bond info for atoms (reads all conect lines regardless of HETATM status)
		 * @param LoadedString The PDB file loaded as a UE-type string
		 * @return An Unreal string that contains the conect info.
		*/
		static TMap<FString, FString> ConectInfo(FString& LoadedString);

	UFUNCTION(BlueprintCallable)
		/**
		 * Generates single bonds
		 * @param ConectString The PDB file CONECT data loaded as a UE-type string
		 * @param AtomIndexCoordMap A map (dictionary) containing atom indices (keys) and their coordinates as a string (value) - Found from AtomIndexAndCoordMap
		 * @param CentroidCoord Centroid of the pdb file, calculated using UToolsFunctionLibrary::Centroid
		 * @return An Unreal string that contains the conect info.
		*/
		static void GenBondSingle(FString& ConectString, TMap<int32, FString>& AtomIndexCoordMap, FVector& CentroidCoord);
};
