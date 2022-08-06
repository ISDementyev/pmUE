// See GPL-v3.0 LICENSE.MD in GitHub repo.

#pragma once

#include "Tools.h"

/**
 * Removes all hydrogens
*/
static void RemoveHydrogens()
{
    //// find all hydrogen atoms and collect them in an array
    //TArray<AActor*> AllHydrogens;
    //UGameplayStatics::GetAllActorsWithTag(GetWorld(), "AtomH", AllHydrogens);

    //if (AllHydrogens.Num() > 0) // only execute if there are hydrogens in the molecule
    //{
    //    // destroy (remove) all hydrogen atoms in the "game"
    //    for (int i = 0; i < AllHydrogens.Num(); i++)
    //    {
    //        AllHydrogens[i]->Destroy();
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Called RemoveHydrogens() but no hydrogens are in the molecule"));
    //}
    ;
}


/**
 * Counts the number of atoms in the pdb file
 * @param FileName Name of the pdb file (string)
 * @param OnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
 * @return Integer - number of atoms in pdb file
*/
int Tools::NumberOfAtoms(std::string FileName, bool OnlyAtom)
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
std::tuple<std::vector<std::string>, std::vector<std::vector<double>>>
    Tools::AcquireCoordinates(std::string FileName, bool OnlyAtom, bool Verbosity) // should return a map (dictionary) of the atoms and their corresponding decimal coordinates
{
    std::tuple<std::vector<std::string>, std::vector<std::vector<double>>> AtomCoordinates; // dictionary for atom coordinates
    std::vector<std::string> AtomSymbols;
    std::vector<std::vector<double>> PositionVectors;
    std::fstream MyFile;

    MyFile.open(FileName, std::ios::in);

    if (MyFile.is_open())
    {
        std::string line;
        while (getline(MyFile, line)) // "scanner" loop
        {
            std::string CheckAtom = line.substr(0, 4); // substr returns first 4 characters of the current line, we are looking for "ATOM", in conjuction with OnlyAtom bool
            //std::cout << "Check Atom: " << CheckAtom << "\n"; //for debugging

            if (OnlyAtom && CheckAtom == "ATOM")
            {
                std::vector<double> PositionVector; // (will be a) 1x3 vector containing the atom coordinates
                std::string AtomName = line.substr(13, 1);

                double X = std::stod(line.substr(32, 6));
                double Y = std::stod(line.substr(40, 6));
                double Z = std::stod(line.substr(48, 6));

                // std::cout << "X: " << x << "\n"; // for debugging

                PositionVector.push_back(X);
                PositionVector.push_back(Y);
                PositionVector.push_back(Z);

                //std::cout << "Atom Name: " << AtomName << "\n"; // for debugging

                AtomSymbols.push_back(AtomName);
                PositionVectors.push_back(PositionVector);
            }
            //std::cout << "Reading line: " << line << std::endl; // for debugging
        }

        AtomCoordinates = std::make_tuple(AtomSymbols, PositionVectors);
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
std::unordered_set<std::string> Tools::UniqueElements(std::string FileName)
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
std::map<std::string, std::vector<int>> Tools::GetElementColoursRGB()
{
    return Tools::ElementColoursRGB;
}

/**
 * Setter function to modify the ElementColoursRGB (allows user to edit/add element colour)
 * @param AtomSymbol The element's atomic symbol
 * @param RGBVector The vector containing the RGB colour info that the user wants to add
*/
void Tools::SetAnElementColourRGB(std::string AtomSymbol, std::vector<int>& RGBVector)
{
    bool Valid = true;

    for (int Value : RGBVector)
    {
        if (Value < 0 || Value > 255) // if invalid RGB value
        {
            Valid = false;
            break; // for efficiency
        }
    }

    if (RGBVector.size() != 3)
    {
        Valid = false;
    }

    if (Valid)
    {
        ElementColoursRGB[AtomSymbol] = RGBVector;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SetAnElementColourRGB had an improper RGB vector input."));
    }
}

/**
 * Returns dictionary of van der Waals radii of atoms
 * @return The specified dictionary, VDWRadii.
*/
std::map<std::string, double> Tools::GetVDWRadii()
{
    return VDWRadii;
}

/**
 * Sets a van der Waals radius for a specific atomic symbol in the VDWRadii dictionary.
 * @param AtomSymbol The symbol for the atom's element.
 * @param Radius The radius to set.
*/
void Tools::SetVDWRadius(std::string AtomSymbol, double Radius)
{
    if (Radius > 0)
    {
        VDWRadii[AtomSymbol] = Radius;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SetVDWRadius had an input radius lower than zero."));
    }
}

/**
 * Returns the radius scale to be put on each atom - used for relative enlargening/shrinking of atoms
 * @return RadiiScale - Radius scale variable
*/
int Tools::GetRadiiScale()
{
    return RadiiScale;
}

/**
 * Overwrites radius scale variable - useful for increasing/decreasing atoms at will.
 * @param NewScale The new value for the scale you'd like to set.
*/
void Tools::SetRadiiScale(int NewScale)
{
    if (NewScale != RadiiScale)
    {
        RadiiScale = NewScale;
    }
}

/**
 * Default Constructor.
*/
Tools::Tools()
{
}

/**
 * Default Destructor.
*/
Tools::~Tools()
{
}
