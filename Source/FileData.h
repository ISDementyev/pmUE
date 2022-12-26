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

public:
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
	FVector CentroidCoordinate = UToolsFunctionLibrary::Centroid(PDBContent, false);

};
