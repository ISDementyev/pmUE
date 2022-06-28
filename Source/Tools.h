/**
* A file containing important tools that will be used within the plugin
*/

#pragma once

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <iostream>
#include <fstream> // used to read pdb files
#include <string> // used for string processing
#include <vector> // used for atom coordinates (c++ vector creation)
#include <map> // used for creating dictionaries (maps) of atoms (keys) and their respective position vector (values)

/**
* Removes all hydrogens
*/
void AtomManager::RemoveHydrogens()
{
    // find all hydrogen atoms and collect them in an array
    TArray<AActor*> AllHydrogens;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "AtomH", AllHydrogens);

    if (AllHydrogens.Num() > 0) // only execute if there are hydrogens in the molecule
    {
        // destroy (remove) all hydrogen atoms in the "game"
        for (int i; i < AllHydrogens.Num(); i++)
        {
            AllHydrogens[i]->Destroy();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Called RemoveHydrogens() but no hydrogens are in the molecule"));
    }
}

/**
 * Counts the number of atoms in the pdb file
 * @param FileName Name of the pdb file (string)
 * @param OnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
 * @return Integer - number of atoms in pdb file
*/
int NumberOfAtoms(std::string FileName, bool OnlyAtom = true)
{
    std::fstream MyFile;
    int counter = 0;

    MyFile.open(FileName, std::ios::in);

    if (MyFile.is_open())
    {
        std::string line;
        while (getline(MyFile, line))
        {
            std::string CheckAtom = line.substr(0, 6); // checks if the line starts with "ATOM", in conjuction with OnlyAtom bool

            //std::cout << "CheckAtom: " << CheckAtom << "\n"; // for debugging

            if (OnlyAtom && CheckAtom == "ATOM  ")
            {
                counter += 1;
            }
            else if (!OnlyAtom && (CheckAtom == "ATOM  " || CheckAtom == "HETATM"))
            {
                counter += 1;
            }
        }
        MyFile.close();
    }
    else
    {
        std::cout << "Cannot find/open file of name: " << FileName << std::endl;
    }

    return counter;
}

/**
 * Acquires coordinates of all atoms from a pdb file
 * @param FileName Name of the pdb input file (string)
 * @param OnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
 * @param Verbosity Tells the function to be verbose or not (true for verbosity)
 * @return Dictionary (map) containing atom names (key) and their coordinates (value)
*/
std::map<std::string, std::vector<double>> AcquireCoordinates(std::string FileName, bool OnlyAtom = true, bool Verbosity = false) // should return a map (dictionary) of the atoms and their corresponding decimal coordinates
{
    std::map<std::string, std::vector<double>> AtomCoordinates; // dictionary for atom coordinates
    std::fstream MyFile;

    MyFile.open(FileName, std::ios::in);

    if (MyFile.is_open())
    {
        std::string line;
        while (getline(MyFile, line)) // "scanner" loop
        {
            std::string CheckAtom = line.substr(0, 4); // substr returns first 4 characters of the current line, we are looking for "ATOM", in conjuction with OnlyAtom bool
            //std::cout << "Check Atom: " << CheckAtom << "\n"; //for debugging

            std::vector<double> PositionVector; // (will be a) 1x3 vector containing the atom coordinates
            std::string AtomName;

            if (OnlyAtom && CheckAtom == "ATOM")
            {
                std::string AtomName = line.substr(13, 4);

                double X = std::stod(line.substr(32, 6));
                double Y = std::stod(line.substr(40, 6));
                double Z = std::stod(line.substr(48, 6));

                // std::cout << "X: " << x << "\n"; // for debugging

                PositionVector.push_back(X);
                PositionVector.push_back(Y);
                PositionVector.push_back(Z);

                //std::cout << "Atom Name: " << AtomName << "\n"; // for debugging
            }

            AtomCoordinates[AtomName] = PositionVector;

            //std::cout << "Reading line: " << line << std::endl; // for debugging
        }
        MyFile.close();
    }
    else
    {
        std::cout << "Cannot find/open file of name: " << FileName << std::endl;
    }

    return AtomCoordinates;
}
