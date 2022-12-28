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

	FString FileName{ "Methane.pdb" };
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString(FileName);
	FVector CentroidCoordinate = UToolsFunctionLibrary::Centroid(PDBContent, false);

};
