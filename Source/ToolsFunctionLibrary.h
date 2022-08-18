// See GPL-v3.0 LICENSE in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolsFunctionLibrary.generated.h"

/**
 * Tools blueprint function library
 */
UCLASS()
class PMUEINTEGRATION_API UToolsFunctionLibrary : public UBlueprintFunctionLibrary
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
		 * Counts the number of atoms in the pdb file
		 * @param LoadedString String loaded from pdb file
		 * @param bOnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
		 * @return An integer number of atoms in pdb file
		*/
		static int32 NumberOfAtoms(FString& LoadedString, bool bOnlyAtom = true);
	
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
		static FVector GetElementColourRGB(FString ChemicalSymbol);
		
		UFUNCTION(BlueprintCallable)
		/**
		 * Returns van der Waals radius of input atom, based on the averages of Tables 1, 2 and 7 in "Van der Waals Radii of Elements" (Batsanov, 2001).
		 * @param ChemicalSymbol The atom's chemical symbol
		 * @return The van der Waals radius of the atom (in nm)
		*/
		static float GetVDWRadius(FString ChemicalSymbol);
};
