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
#include <unordered_set> // used for making sets (to list unique elements found in a pdb file, among other things)

/**
 * Tools class, containing useful functions and attributes for analyzing PDB files
*/
class Tools {
public:
    /**
     * Removes all hydrogens
    */
    static void AtomManager::RemoveHydrogens()
    {
       // find all hydrogen atoms and collect them in an array
       TArray<AActor*> AllHydrogens;
       UGameplayStatics::GetAllActorsWithTag(GetWorld(), "AtomH", AllHydrogens);

       if (AllHydrogens.Num() > 0) // only execute if there are hydrogens in the molecule
       {
           // destroy (remove) all hydrogen atoms in the "game"
           for (int i = 0; i < AllHydrogens.Num(); i++)
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
    static int NumberOfAtoms(std::string FileName, bool OnlyAtom = true)
    {
        std::fstream MyFile;
        int counter = 0;

        MyFile.open(FileName, std::ios::in);

        if (MyFile.is_open())
        {
            std::string line;
            while (getline(MyFile, line))
            {
                std::string CheckAtom = line.substr(0, 6); // checks if the line starts with "ATOM  ", in conjuction with OnlyAtom bool

                //std::cout << "CheckAtom: " << CheckAtom << "\n"; // for debugging

                if (OnlyAtom && CheckAtom == "ATOM  ")
                {
                    counter++;
                }
                else if (!OnlyAtom && (CheckAtom == "ATOM  " || CheckAtom == "HETATM"))
                {
                    counter++;
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
    static std::map<std::string, std::vector<double>> AcquireCoordinates(std::string FileName, bool OnlyAtom = true, bool Verbosity = false) // should return a map (dictionary) of the atoms and their corresponding decimal coordinates
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

    /**
     * Returns set of all unique elements found in the pdb - useful for blueprint generation/modification
     * @param FileName The pdb file's name
     * @return Set of all unique elements (not including hetatms)
     */
    static std::unordered_set<std::string> UniqueElements(std::string FileName)
    {
        // initialize set
        std::unordered_set<std::string> UniqueElementNames;

        std::fstream MyFile;

        MyFile.open(FileName, std::ios::in);

        if (MyFile.is_open())
        {
            std::string line;
            while (getline(MyFile, line)) // "scanner" loop
            {
                if (line.substr(0, 4) == "ATOM")
                {
                    std::string Element = line.substr(13, 1);
                    UniqueElementNames.insert(Element);
                }
            }
            MyFile.close();
        }
        else
        {
            std::cout << "Cannot find/open file of name: " << FileName << std::endl;
        }

        return UniqueElementNames;
    }

    /**
     * Getter function for the ElementColoursRGB vector
     * @return ElementColoursRGB vector (all elements that have distinct colours)
    */
    std::map<std::string, std::vector<int>> GetElementColoursRGB()
    {
        return ElementColoursRGB;
    }

    /**
     * Setter function to modify the ElementColoursRGB (allows user to edit/add element colour)
     * @param Name The element's atomic symbol
     * @param RGBVector The vector containing the RGB colour info that the user wants to add
    */
    void SetAnElementColourRGB(std::string Name, std::vector<int>& RGBVector)
    {
        bool Valid = true;

        for (int Value : RGBVector)
        {
            if (Value < 0 || Value > 255)
            {
                Valid = false;
            }
        }

        if (RGBVector.size() != 3)
        {
            Valid = false;
        }

        if (Valid)
        {
            ElementColoursRGB[Name] = RGBVector;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SetAnElementColourRGB had an improper RGB vector input."));
        }
    }

private:
    /**
    * ElementColoursRGB - A dictionary containing RGB values for
    */
    std::map<std::string, std::vector<int>>
        ElementColoursRGB = { {"O", {240, 0, 0}},
            {"H", {255, 255, 255}},
            {"C", {200, 200, 200}},
            {"N", {143, 143, 255}},
            {"S", {255, 200, 50}},
            {"P", {255, 165, 0}},
            {"Fe", {210, 105, 30}},
            {"Zn", {211, 211, 211}} };
};
