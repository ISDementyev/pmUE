#pragma once

#include "CoreMinimal.h"
#include "ToolsFunctionLibrary.h"

/**
 * FileData class: Contains relevant data from the current PDB file 
 */
class PMUEDEVELOPMENTTWO_API FileData
{
public:
	FileData();
	~FileData();

	bool AtomOnly{ false };
	FString FileName{ "AceticAcid.pdb" };
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString(FileName);
	FVector CentroidCoordinate = UToolsFunctionLibrary::Centroid(PDBContent, AtomOnly);
	int32 NAtoms = UToolsFunctionLibrary::NumberOfAtoms(PDBContent, AtomOnly);
	TMap<int32, FString> IndexAndCoord = UToolsFunctionLibrary::AtomIndexAndCoordMap(PDBContent, NAtoms, AtomOnly);

};
