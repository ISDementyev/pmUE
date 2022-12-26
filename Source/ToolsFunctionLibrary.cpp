// See GPL-v3.0 LICENSE in GitHub repo.

#include "ToolsFunctionLibrary.h"

TMap<FString, float> VDWRadii{ {TEXT("H"), 1.2f}, {TEXT("C"), 1.7f}, {TEXT("N"), 1.5f}, {TEXT("O"), 1.4f},
		{TEXT("S"), 1.8f}, {TEXT("P"), 2.08f}, {TEXT("Fe"), 2.0f}, {TEXT("Zn"), 2.1f}, {TEXT("H "), 1.2f},
	{TEXT("C "), 1.7f}, {TEXT("N "), 1.5f}, {TEXT("O "), 1.4f},
		{TEXT("S "), 1.8f}, {TEXT("P "), 2.08f} };

int32 SpreadOutFactor{ 75 };

/**
 * Converts file content into string
 * @param Filename Name of the file
 * @return The file's content as an FString
*/
FString UToolsFunctionLibrary::ConvFileToString(FString Filename)
{
	FString Directory = FPaths::GameSourceDir();
	IFileManager& File = IFileManager::Get();
	FString LoadedString;

	if (File.DirectoryExists(*Directory))
	{
		FString MyFile = Directory + "/" + Filename;
		FFileHelper::LoadFileToString(LoadedString, *MyFile);
	}

	return LoadedString;
}

/**
 * Counts the number of atoms in the pdb file
 * @param LoadedString String loaded from pdb file
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
 * @return An integer number of atoms in pdb file
*/
int32 UToolsFunctionLibrary::NumberOfAtoms(FString& LoadedString, bool bOnlyAtom)
{
	int32 Counter{ 0 };

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  "))) 
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			Counter++;
		}
	}

	return Counter;
}

/**
 * Counts the number of atoms in the pdb file, specific to the spawner's element
 * @param LoadedString String loaded from pdb file
 * @param CurrentElement The element corresponding to the spawner
 * @param bOnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
 * @return An integer number of atoms in pdb file
*/
int32 UToolsFunctionLibrary::NumberOfAtomsSingleElement(FString& LoadedString, FString& CurrentElement, bool bOnlyAtom)
{
	int32 Counter{ 0 };

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);
		FString Element = LoadedString.Mid(i + 13, 2);

		if ((Element == CurrentElement) && ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM"))))))
		{
			Counter++;
		}
	}

	return Counter;
}

/**
 * Returns set of all unique elements found in the pdb - useful for blueprint generation/modification
 * @param LoadedString String loaded from pdb file
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
 * @return Set of all unique elements (not including hetatms)
*/
TSet<FString> UToolsFunctionLibrary::UniqueElements(FString& LoadedString, bool bOnlyAtom)
{
	TSet<FString> UniqueChemicalSymbols;

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  "))) 
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			UniqueChemicalSymbols.Add(LoadedString.Mid(i + 13, 2));
		}
	}

	return UniqueChemicalSymbols;
}

/**
 * Getter function for a 3-vector corresponding to the input atom's color
 * @param ChemicalSymbol The atom's chemical symbol, as a string
 * @return RGB vector corresponding to the colour of the input atom (pink if unknown symbol)
*/
FVector UToolsFunctionLibrary::GetElementColourRGB(FString& ChemicalSymbol)
{
	if (ChemicalSymbol == TEXT("H") || ChemicalSymbol == TEXT("H "))
	{
		// white
		return FVector{ 255, 255, 255 };
	}
	else if (ChemicalSymbol == TEXT("C") || ChemicalSymbol == TEXT("C "))
	{
		// black
		return FVector{ 0, 0, 0 };
	}
	else if (ChemicalSymbol == TEXT("N") || ChemicalSymbol == TEXT("N "))
	{
		// blue
		return FVector{ 143, 143, 255 };
	}
	else if (ChemicalSymbol == TEXT("O") || ChemicalSymbol == TEXT("O "))
	{
		// red
		return FVector{ 240, 0, 0 };
	}
	else if (ChemicalSymbol == TEXT("P") || ChemicalSymbol == TEXT("P "))
	{
		// brownish-yellow
		return FVector{ 255, 165, 0 };
	}
	else if (ChemicalSymbol == TEXT("S") || ChemicalSymbol == TEXT("S "))
	{
		// yellow
		return FVector{ 255, 200, 50 };
	}
	else if (ChemicalSymbol == TEXT("Fe"))
	{
		// brown
		return FVector{ 210, 105, 30 };
	}
	else if (ChemicalSymbol == TEXT("Zn"))
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
float UToolsFunctionLibrary::GetVDWRadius(FString ChemicalSymbol, TMap<FString, float>& VDWRadiiMap)
{
	return VDWRadiiMap[ChemicalSymbol];
}

/**
 * Reads and returns coordinates from PDB
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param Transforms The Coordinate array
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
*/
void UToolsFunctionLibrary::GetCoordinates(FString& LoadedString, TArray<FTransform>& Transforms, bool bOnlyAtom)
{
	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  "))) 
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Beginning to spawn generic atom"));

			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

			Transforms.Add(FTransform(FVector(X, Y, Z) * SpreadOutFactor));

			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Spawning generic atom @ (%f, %f, %f)"), X, Y, Z);
		}
	}
}

/**
 * Returns the centroid of the PDB, based on atom positions
 * @param LoadedString The string contents of the PDB file
 * @param bOnlyAtom
 * @return PDB file's centroid coordinate
*/
FVector UToolsFunctionLibrary::Centroid(FString& LoadedString, bool bOnlyAtom)
{
	// initialize centroid coords
	float XC = 0, YC = 0, ZC = 0;

	// initialize normalization constant
	int32 Counter{ 0 };

	// loop through pdb file
	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  "))) 
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

			XC += X;
			YC += Y;
			ZC += Z;
			Counter++;
		}
	}

	return FVector(XC, YC, ZC) / Counter;
}

/**
 * Reads and spawns centroid-corrected coordinates from PDB
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param Transforms The coordinate array
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
 * @param Centroid Centroid calculated using UToolsFunctionLibrary::Centroid
*/
void UToolsFunctionLibrary::CentroidCorrected(FString& LoadedString, TArray<FTransform>& Transforms, 
	FVector& CentroidCoord, bool bOnlyAtom)
{
	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  "))) 
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Beginning to spawn generic atom"));

			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

			Transforms.Add(FTransform((FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor));

			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Spawning generic atom @ (%f, %f, %f)"), X * SpreadOutFactor, Y * SpreadOutFactor, Z * SpreadOutFactor);
		}
	}
}

/**
 * Centroid-Corrected Element-Specific Atom Spawner: Reads and returns centroid-corrected coordinates from PDB
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param Transforms The coordinate array
 * @param CentroidCoord Centroid of the pdb file, can be calculated using UToolsFunctionLibrary::Centroid
 * @param Element The atom's element 
 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards others (e.g. "HETATM" and "ANISOU" rows, etc.)
*/
void UToolsFunctionLibrary::CCESAtomGeneration(FString& LoadedString, TArray<FTransform>& Transforms,
	FVector& CentroidCoord, FString& Element, bool bOnlyAtom)
{
	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			if (LoadedString.Mid(i + 13, 2).Equals(Element))
			{
				// debugging
				UE_LOG(LogTemp, Warning, TEXT("Beginning to spawn %s atom"), *Element);

				// extract coordinates
				float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
				float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
				float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

				// add coordinates to array list
				Transforms.Add(FTransform((FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor));

				// debugging
				UE_LOG(LogTemp, Warning, TEXT("Spawning %s atom @ (%f, %f, %f)"), *Element, X * SpreadOutFactor, Y * SpreadOutFactor, Z * SpreadOutFactor);
			}
		}
	}
}

void UToolsFunctionLibrary::UpdateCoordinates(FString& LoadedString, FVector& CentroidCoord, UInstancedStaticMeshComponent* ISMC, bool bOnlyAtom)
{
	int32 AtomIndex = 0;
	int32 BoxDim = 100;

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered for loop"));
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Currently at an ATOM/HETATM row in UpdateCoordinates"));

			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

			float VDWRad = UToolsFunctionLibrary::GetVDWRadius(LoadedString.Mid(i + 13, 2), VDWRadii);
			float TranslationFactor = (BoxDim / 2) * (VDWRad - 1) * SpreadOutFactor;

			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Update coordinates, AtomIndex=%d"), AtomIndex);
			
			ISMC->UpdateInstanceTransform(AtomIndex, FTransform(FQuat(0.0f, 0.0f, 0.0f, 1.0f),
				(FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor, FVector(VDWRad)), true, true, true);

			FTransform InstanceTransform;
			ISMC->GetInstanceTransform(0, InstanceTransform, true);

			AtomIndex++;
		}
	}
}

/**
 * Returns indices of atoms from the PDB file string contents
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
 * @return Map (dictionary) of atom names and their indices from the pdb - format is (Index: AtomName)
*/
TMap<int32, FString> UToolsFunctionLibrary::AtomNameAndIndex(FString& LoadedString, bool bOnlyAtom)
{
	int32 Index{ 0 };
	TMap<int32, FString> AtomNamesAndIndices;

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			// debugging
			//UE_LOG(LogTemp, Warning, TEXT("Adding atom..."));
			
			AtomNamesAndIndices.Add(Index, LoadedString.Mid(i + 13, 2));
			Index++;
		}
	}

	// debugging
	//UE_LOG(LogTemp, Warning, TEXT("Number of atoms in NameIndex map: %d"), AtomNamesAndIndices.Num());

	return AtomNamesAndIndices;
}

