// See GPL-v3.0 LICENSE in GitHub repo.


#include "ToolsFunctionLibrary.h"

/**
 * Counts the number of atoms in the pdb file
 * @param FileName Name of the pdb file (string)
 * @param OnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
 * @return An integer number of atoms in pdb file
*/
int64 UToolsFunctionLibrary::NumberOfAtoms(FString Filename, bool OnlyAtom)
{
	return -1;
}

/**
 * Returns set of all unique elements found in the pdb - useful for blueprint generation/modification
 * @param FileName The pdb file's name
 * @return Set of all unique elements (not including hetatms)
*/
TSet<FString> UToolsFunctionLibrary::UniqueElements(FString Filename)
{
	return TSet<FString>();
}

/**
 * Getter function for a 3-vector corresponding to the input atom's color
 * @param ChemicalSymbol The atom's chemical symbol, as a string
 * @return RGB vector corresponding to the colour of the input atom (pink if not assigned)
*/
FVector UToolsFunctionLibrary::GetElementColourRGB(FString AtomicSymbol)
{
	if (AtomicSymbol == TEXT("H"))
	{
		// white
		return FVector{ 255, 255, 255 }; 
	}
	else if (AtomicSymbol == TEXT("C")) 
	{
		// black
		return FVector{ 0, 0, 0 }; 
	} 
	else if (AtomicSymbol == TEXT("N")) 
	{
		// blue
		return FVector{ 143, 143, 255 }; 
	} 
	else if (AtomicSymbol == TEXT("O")) 
	{
		// red
		return FVector{ 240, 0, 0 }; 
	} 
	else if (AtomicSymbol == TEXT("P")) 
	{
		// brownish-yellow
		return FVector{ 255, 165, 0 }; 
	} 
	else if (AtomicSymbol == TEXT("S")) 
	{
		// yellow
		return FVector{ 255, 200, 50 }; 
	} 
	else if (AtomicSymbol == TEXT("Fe")) 
	{
		// brown
		return FVector{ 210, 105, 30 }; 
	} 
	else if (AtomicSymbol == TEXT("Zn")) 
	{
		// silver-gray
		return FVector{ 100, 100, 100 }; 
	} 
	else 
	{ 
		// pink
		return FVector{ 255, 16, 240 }; 
	} 
}

/**
 * Returns van der Waals radius of input atom, based on the averages of Tables 1, 2 and 7 in "Van der Waals Radii of Elements" (Batsanov, 2001).
 * @param ChemicalSymbol The atom's chemical symbol
 * @return The van der Waals radius of the atom (in nm)
*/
double UToolsFunctionLibrary::GetVDWRadius(FString AtomSymbol)
{
	return 0.0;
}