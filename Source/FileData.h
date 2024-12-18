// Fill out your copyright notice in the Description page of Project Settings.

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

	// All radii have been scaled down by multiplying 0.2/1.7
	bool AtomOnly{ false };
	FString FileName{ "6a5j-small.pdb" }; //6A5J is a small peptide, I shortened it to Ile-Lys-Lys
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString(FileName); // The FString of the PDB file's contents
	FVector CentroidCoordinate = UToolsFunctionLibrary::Centroid(PDBContent, AtomOnly); // The coordinates of the centroid
	int32 NAtoms = UToolsFunctionLibrary::NumberOfAtoms(PDBContent, AtomOnly); // Number of atoms in the PDB
	TMap<int32, FString> IndexAndCoord = UToolsFunctionLibrary::AtomIndexAndCoordMap(PDBContent, NAtoms, AtomOnly);
	TMap<FString, FString> BondInfo = UToolsFunctionLibrary::ConectInfo(PDBContent);
	int32 SpreadOutFactor{ 1 };
	bool bDebug{ false };
};
