// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolsFunctionLibrary.h"
#include "FileData.h"

TMap<FString, float> VDWRadii{ {TEXT("H"), 1.2f}, {TEXT("C"), 1.7f}, {TEXT("N"), 1.5f}, {TEXT("O"), 1.4f},
		{TEXT("S"), 1.8f}, {TEXT("P"), 2.08f}, {TEXT("Fe"), 2.0f}, {TEXT("Zn"), 2.1f}, {TEXT("H "), 1.2f},
	{TEXT("C "), 1.7f}, {TEXT("N "), 1.5f}, {TEXT("O "), 1.4f},
		{TEXT("S "), 1.8f}, {TEXT("P "), 2.08f} };

FileData FD;
int32 SpreadOutFactor = FD.SpreadOutFactor;
bool bDebug = FD.bDebug;

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

	//UE_LOG(LogTemp, Warning, TEXT("NumberOfAtoms: %d"), Counter);
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
	// debug
	UE_LOG(LogTemp, Warning, TEXT("NumberOfAtomsSingleElement is now running."));

	int32 Counter{ 0 };

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);
		FString Element = LoadedString.Mid(i + 77, 2);

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
 * Helper function - Returns the centroid of the PDB, based on atom positions
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

	//UE_LOG(LogTemp, Warning, TEXT("Centroid coord: %f, %f, %f"), XC, YC, ZC);

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
	// debug
	UE_LOG(LogTemp, Warning, TEXT("CentroidCorrected is now running"));

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
 * @param CentroidCoord Centroid of the pdb file, calculated using UToolsFunctionLibrary::Centroid
 * @param Element The atom's element 
 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards others (e.g. "HETATM" and "ANISOU" rows, etc.)
*/
void UToolsFunctionLibrary::CCESAtomGeneration(FString& LoadedString, TArray<FTransform>& Transforms,
	FVector& CentroidCoord, FString& Element, bool bOnlyAtom)
{
	// debug
	//UE_LOG(LogTemp, Warning, TEXT("CCESAtomGeneration is now running"));
	//UE_LOG(LogTemp, Warning, TEXT("Length of LoadedString: %d"), LoadedString.Len());
	
	TCHAR ElementNoWS = Element[0];
	//UE_LOG(LogTemp, Warning, TEXT("TCHAR ElementNoWS: %c"), ElementNoWS);

	// remove whitespaces; check if the element is one or two-lettered
	// this should automatically Element to a usable var for checks, use TrimmedElement for everything else
	FString TrimmedElement = Element.TrimEnd();
	bool bOneLetter = false;

	// In this case, if the input (FString& Element) had a ws, 
	// then TrimmedElement does not, indicating one-letter element name
	if (TrimmedElement != Element)
	{
		bOneLetter = true;
	}

	
	// main code
	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			// debug
			//UE_LOG(LogTemp, Warning, TEXT("CCESAtomGeneration: Entering first 'if' statement"));
			
			FString CheckElementStr = LoadedString.Mid(i + 77, 2);
			if (bOneLetter)
			{
				TCHAR CheckElementStrChar = CheckElementStr[0];

				if (CheckElementStrChar == ElementNoWS)
				{
					// debugging
					//UE_LOG(LogTemp, Warning, TEXT("CCESAtomGeneration: Beginning to spawn %s atom"), *Element);

					// extract coordinates
					float X = FCString::Atof(*LoadedString.Mid(i + 31, 7));
					float Y = FCString::Atof(*LoadedString.Mid(i + 39, 7));
					float Z = FCString::Atof(*LoadedString.Mid(i + 47, 7));

					// add coordinates to array list
					Transforms.Add(FTransform((FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor));

					// debugging
					//UE_LOG(LogTemp, Warning, TEXT("CCESAtomGeneration: Spawning %s atom @ (%f, %f, %f)"), *Element, X * SpreadOutFactor, Y * SpreadOutFactor, Z * SpreadOutFactor);
				}
			}
			else
			{
				if (CheckElementStr.Equals(TrimmedElement))
				{
					// debugging
					//UE_LOG(LogTemp, Warning, TEXT("CCESAtomGeneration: Beginning to spawn %s atom"), *Element);

					// extract coordinates
					float X = FCString::Atof(*LoadedString.Mid(i + 31, 7));
					float Y = FCString::Atof(*LoadedString.Mid(i + 39, 7));
					float Z = FCString::Atof(*LoadedString.Mid(i + 47, 7));

					// add coordinates to array list
					Transforms.Add(FTransform((FVector(X, Y, Z) - CentroidCoord) * SpreadOutFactor));

					// debugging
					//UE_LOG(LogTemp, Warning, TEXT("CCESAtomGeneration: Spawning %s atom @ (%f, %f, %f)"), *Element, X * SpreadOutFactor, Y * SpreadOutFactor, Z * SpreadOutFactor);
				}
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
 * Returns indices of atoms and their respective coordinates
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param NumberOfAtoms Number of atoms in the PDB
 * @param bOnlyAtom Boolean that, if true, only scans "ATOM" rows and disregards heteroatoms ("HETATM" rows)
 * @return Map (dictionary) of atom names and their indices from the pdb - format is (Index: AtomName)
*/
TMap<int32, FString> UToolsFunctionLibrary::AtomIndexAndCoordMap(FString& LoadedString, int32 NumberOfAtoms, bool bOnlyAtom)
{
	int32 Index{ 0 };
	TMap<int32, FString> AtomIndicesAndCoords;

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{			
			AtomIndicesAndCoords.Add(Index, LoadedString.Mid(i + 30, 24));
			Index++;

			//debugging
			//UE_LOG(LogTemp, Warning, TEXT("Adding key, value into IndexAndCoord TMap: (%d, %s)"), Index, *LoadedString.Mid(i + 30, 24));
		}

		if (Index == NumberOfAtoms)
		{
			break;
		}
	}

	return AtomIndicesAndCoords;
}

/**
 * Returns UE Array containing indices of atoms that AtomIndex's atom is connected to.
 * @param LoadedString The PDB file loaded as a UE-type string
 * @param ElementName The element
 * @param bOnlyAtom Boolean that, if true (by default), only scans "ATOM" rows and disregards heteroatoms (e.g. "HETATM" and "ANISOU" rows, etc.)
 * @return UE vector containing indices of atoms that AtomIndex's atom is connected to
*/
TArray<int32> UToolsFunctionLibrary::ElementIndices(FString& LoadedString, FString ElementName, bool bOnlyAtom)
{
	TArray<int32> Indices;
	//const TCHAR* Delim = TEXT(" ");
	ElementName = ElementName.Replace(TEXT(" "), TEXT(""), ESearchCase::CaseSensitive);

	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);

		if ((bOnlyAtom && ScannedStr.Equals(TEXT("ATOM  ")))
			|| (!bOnlyAtom && (ScannedStr.Equals(TEXT("ATOM  ")) || ScannedStr.Equals(TEXT("HETATM")))))
		{
			FString CurrentRow = LoadedString.Mid(i, 30);

			FString ElementCurrent = LoadedString.Mid(i + 77, 2);
			ElementCurrent = ElementCurrent.Replace(TEXT(" "), TEXT(""), ESearchCase::CaseSensitive);

			if (ElementCurrent.Equals(ElementName))
			{
				int32 AtomSerial = FCString::Atoi(*LoadedString.Mid(i + 6, 4));
				Indices.Add(AtomSerial);
			}
		}
	}

	return Indices;
}

/**
 * Helper function, retrieves bond info for atoms (reads all conect lines regardless of HETATM status)
 * @param LoadedString The PDB file loaded as a UE-type string
 * @return An Unreal string that contains the conect info.
*/
TMap<FString, FString> UToolsFunctionLibrary::ConectInfo(FString& LoadedString)
{
	FString ConectInfoStr;
	TArray<FString> ConectArr;
	TMap<FString, FString> ConectMap;
	const TCHAR* Delim = TEXT(" ");
	const TCHAR* DelimComma = TEXT(",");

	// returns the raw CONECT data as one big str
	for (int32 i = 0; i < LoadedString.Len(); i++)
	{
		FString ScannedStr = LoadedString.Mid(i, 6);
		//UE_LOG(LogTemp, Warning, TEXT("ScannedStr: (%s)"), *ScannedStr);

		if (ScannedStr.Equals(TEXT("CONECT")))
		{
			ConectInfoStr = LoadedString.Mid(i);
			//ConectInfoStr = ConectInfoStr.Replace(TEXT("CONECT"), TEXT(""));
			break;
		}
	}

	ConectInfoStr.ParseIntoArrayLines(ConectArr, true);
	TArray<FString> ConectArrClean; // set up clean arr

	// removes non-CONECT lines from the array by making a new one (ConectArrClean)
	for (int32 i = 0; i < ConectArr.Num(); i++)
	{
		FString CurrentRow = ConectArr[i];
		if (CurrentRow.Contains(TEXT("CONECT")))
		{
			ConectArrClean.Add(CurrentRow.Mid(6));
		}
	}

	// actually generates the TMap return variable
	for (int32 j = 0; j < ConectArrClean.Num(); j++)
	{
		TArray<FString> ConectRow; // contains something like: 1    2     3     7
		ConectArrClean[j].ParseIntoArray(ConectRow, Delim);
		FString Value = ""; // corresponds to CONECT row after first atom number

		for (int32 k = 1; k < ConectRow.Num(); k++)
		{
			Value.Append(ConectRow[k].Append(TEXT(","))); // create value string containing all atoms bonded, separated by , delim
		}

		// debugging
		//UE_LOG(LogTemp, Warning, TEXT("ToolsFunctionLibrary: ConectInfo: ConectRow[0], Value: (%s, %s)"), *ConectRow[0], *Value);
		ConectMap.Add(ConectRow[0], Value);
	}

	TSet<FString> KeepTrack; // Keeps track of atoms iterated through so far.
	TMap<FString, FString> FinalMap; // The final, cleaned map.
	for (const TPair<FString, FString>& pair : ConectMap)
	{
		FString NewConectRow; // To store the cleaned (copy-removed) row
		FString CurrentConectRow = pair.Value;
		//UE_LOG(LogTemp, Warning, TEXT("ToolsFunctionLibrary: ConectInfo: CurrentConectRow: %s"), *pair.Value);

		FString Key = pair.Key;
		KeepTrack.Add(",");

		TArray<FString> CurrentConectRowArray;
		CurrentConectRow.ParseIntoArray(CurrentConectRowArray, DelimComma, true);

		for (FString AtomIndex : CurrentConectRowArray)
		{
			if (!KeepTrack.Contains(Key))
			{
				KeepTrack.Add(Key);
			}

			if (!KeepTrack.Contains(AtomIndex))
			{
				KeepTrack.Add(AtomIndex);
				NewConectRow.Append(AtomIndex);
				NewConectRow.AppendChar(*TEXT(","));
			}
		}

		if (NewConectRow.Len() > 0)
		{
			FinalMap.Add(pair.Key, NewConectRow);
			
			//debugging
			//UE_LOG(LogTemp, Warning, TEXT("ToolsFunctionLibrary: ConectInfo: FinalMap key, value: %s, %s"), *pair.Key, *NewConectRow); // final check - output is expected
		}		
	}	

	return FinalMap;
}

/**
 * Generates single bonds
 * @param ConectString The PDB file CONECT data loaded as a UE-type string
 * @param AtomIndexCoordMap A map (dictionary) containing atom indices (keys) and their coordinates as a string (value) - Found from AtomIndexAndCoordMap
 * @param CentroidCoord Centroid of the pdb file, calculated using UToolsFunctionLibrary::Centroid
 * @return An Unreal string that contains the conect info.
*/
void UToolsFunctionLibrary::GenBondSingle(FString& ConectString, TMap<int32, FString>& AtomIndexCoordMap, FVector& CentroidCoord)
{
	// debug
	UE_LOG(LogTemp, Warning, TEXT("GenBondSingle is now running"));
	const TCHAR* Delim = TEXT(" ");

	for (int32 i = 0; i < ConectString.Len(); i++)
	{
		FString ScannedStr = ConectString.Mid(i, 6);

		if (ScannedStr.Equals(TEXT("CONECT")))
		{
			// Parse CONECT row into easy-to-handle array
			FString ConectRow = ConectString.Mid(i, 30);
			TArray<FString> ConectAtoms;
			ConectRow.ParseIntoArray(ConectAtoms, Delim, true);

			// finding index of first atom in CONECT row
			int32 FocusedAtom = FCString::Atoi(*ConectAtoms[1]);

			// finding coordinates of first atom in CONECT row 
			FString FocusedAtomCoordString = AtomIndexCoordMap[FocusedAtom];
			TArray<FString> FocusedAtomCoords;
			FocusedAtomCoordString.ParseIntoArray(FocusedAtomCoords, Delim, true);

			FVector FACNumerical = FVector(FCString::Atof(*FocusedAtomCoords[0]), 
				FCString::Atof(*FocusedAtomCoords[1]), FCString::Atof(*FocusedAtomCoords[2]));

			// iterating through the rest of the atoms in the same row
			for (int32 j = 2; j < ConectAtoms.Num(); j++)
			{
				// finding index of next bonded atom in the same CONECT row
				int32 CurrentBondedAtom = FCString::Atoi(*ConectAtoms[j]);
				FString CBACoordString = AtomIndexCoordMap[CurrentBondedAtom];
				TArray<FString> CBACoords;
				CBACoordString.ParseIntoArray(CBACoords, Delim, true);

				FVector CBANumerical = FVector(FCString::Atof(*CBACoords[0]),
					FCString::Atof(*CBACoords[1]), FCString::Atof(*CBACoords[2]));

				FVector BisectPoint = (FACNumerical + CBANumerical) / 2 - CentroidCoord;

				// spawn single bond @ BisectPoint

			}
		}
	}
}
