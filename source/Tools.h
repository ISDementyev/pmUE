/**
* A file containing important tools that will be used within the plugin
*/

#pragma once

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector> //used for atom coordinates

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
}

/**
* Counts total number of atoms in a pdb file
*/
int NumberOfAtoms(std::string FileName)
{
    std::fstream MyFile;
    int counter = 0;

    MyFile.open(FileName, std::ios::in);

    if (MyFile.is_open())
    {
        std::string line;
        while (getline(MyFile, line) && line.substr(0, 3) != "END")
        {
            std::string CheckAtom = line.substr(0, 4); // checks if the line starts with "ATOM", in conjuction with OnlyAtom bool
            
            if (CheckAtom == "ATOM")
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
* Reads a pdb file
*/
void ReadPdb(std::string FileName, bool OnlyAtom = true)
{
    std::fstream MyFile;
    MyFile.open(FileName, std::ios::in);

    if (MyFile.is_open())
    {
        std::string line;
        while (getline(MyFile, line) && line.substr(0, 3) != "END")
        {
            std::string CheckAtom = line.substr(0, 4); // checks if the line starts with "ATOM", in conjuction with OnlyAtom bool
          
            if (OnlyAtom && CheckAtom == "ATOM")
            {
                // This will iterate through all the rows with atoms and create position vectors to function as coordinates
            }
          
            std::cout << "Reading line: " << line << std::endl;
        }
        MyFile.close();
    }
    else
    {
        std::cout << "Cannot find/open file of name: " << FileName << "\n";
    }
}
