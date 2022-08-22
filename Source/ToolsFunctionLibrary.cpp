// See GPL-v3.0 LICENSE in GitHub repo.


#include "ToolsFunctionLibrary.h"

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

		if (bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
		{
			Counter++;
		}
		else if (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM"))))
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

		if (bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
		{
			UniqueChemicalSymbols.Add(LoadedString.Mid(i + 13, 2));
		}
		else if (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM"))))
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
float UToolsFunctionLibrary::GetVDWRadius(FString ChemicalSymbol)
{
	TMap<FString, float> VDWRadii{ {TEXT("H"), 0.12}, {TEXT("C"), 0.17}, {TEXT("N"), 0.15}, {TEXT("O"), 0.14}, 
		{TEXT("S"), 0.18}, {TEXT("P"), 0.208}, {TEXT("Fe"), 0.20}, {TEXT("Zn"), 0.21} };
	
	return VDWRadii[ChemicalSymbol];
}

/**
 * Reads and returns coordinates from PDB
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param Transforms The Coordinate array
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
*/
void UToolsFunctionLibrary::GetCoordinates(FString& LoadedString, TArray<FTransform>& Transforms, bool bOnlyAtom)
{
	int32 SpreadOutFactor{ 75 };

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if (bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
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
		else if (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM"))))
		{
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

		if (bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
		{
			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));
			
			XC += X;
			YC += Y;
			ZC += Z;
			Counter++;
		}
		else if (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM"))))
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

	return FVector( XC, YC, ZC ) / Counter;
}

/**
 * Reads and returns centroid-corrected coordinates from PDB
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param Transforms The coordinate array
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
 * @param Centroid Centroid calculated using UToolsFunctionLibrary::Centroid
*/
void UToolsFunctionLibrary::CentroidCorrected(FString& LoadedString, TArray<FTransform>& Transforms, FVector& CentroidCoord, bool bOnlyAtom)
{
	int32 SpreadOutFactor{ 75 };

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if (bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
		{
			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Beginning to spawn generic atom"));

			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

			//Transforms.Add(FTransform(FVector(X - CentroidCoord.X, Y - CentroidCoord.Y, Z - CentroidCoord.Z) * SpreadOutFactor));
			Transforms.Add(FTransform((FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor));

			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Spawning generic atom @ (%f, %f, %f)"), X, Y, Z);
		}
		else if (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM"))))
		{
			float X = FCString::Atof(*LoadedString.Mid(i + 32, 6));
			float Y = FCString::Atof(*LoadedString.Mid(i + 40, 6));
			float Z = FCString::Atof(*LoadedString.Mid(i + 48, 6));

			Transforms.Add(FTransform((FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor));

			// debugging
			UE_LOG(LogTemp, Warning, TEXT("Spawning generic atom @ (%f, %f, %f)"), X, Y, Z);
		}
	}
}
