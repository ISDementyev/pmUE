// See GPL-v3.0 LICENSE.MD in GitHub repo.

#pragma once

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include <iostream>
#include <fstream> // used to read pdb files
#include <string> // used for string processing
#include <vector> // used for atom coordinates (c++ vector creation)
#include <map> // used for creating dictionaries (see private access modifier below)
#include <unordered_set> // used for making sets (to list unique elements found in a pdb file, among other things)
#include "CoreMinimal.h"

/**
 * UE Equivalent (UEE) of Tools.h file
 */
class PMUEINTEGRATION_API Tools
{
public:
    /**
     * Default Constructor.
    */
    Tools();

    /*
     * Default Destructor. 
    */
    ~Tools();

    /**
     * Removes all hydrogens
    */
    static void RemoveHydrogens();

    /**
     * Counts the number of atoms in the pdb file
     * @param FileName Name of the pdb file (string)
     * @param OnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
     * @return An integer number of atoms in pdb file
    */
    static int NumberOfAtoms(std::string FileName, bool OnlyAtom = true);

    /**
     * Acquires coordinates of all atoms from a pdb file
     * @param FileName Name of the pdb input file (string)
     * @param OnlyAtom Boolean that, if true, does not consider heteroatoms (HETATM rows in pdb file)
     * @param Verbosity Tells the function to be verbose or not (true for verbosity)
     * @return Dictionary (map) containing atom names (key) and their coordinates (value)
    */
    static std::tuple<std::vector<std::string>, std::vector<std::vector<double>>>
        AcquireCoordinates(std::string FileName, bool OnlyAtom = true, bool Verbosity = false);

    /**
     * Returns set of all unique elements found in the pdb - useful for blueprint generation/modification
     * @param FileName The pdb file's name
     * @return Set of all unique elements (not including hetatms)
     */
    static std::unordered_set<std::string> UniqueElements(std::string FileName);

    /**
     * Getter function for the ElementColoursRGB vector
     * @return ElementColoursRGB vector (all elements that have distinct colours)
    */
    std::map<std::string, std::vector<int>> GetElementColoursRGB();

    /**
     * Setter function to modify the ElementColoursRGB (allows user to edit/add element colour)
     * @param AtomSymbol The element's atomic symbol
     * @param RGBVector The vector containing the RGB colour info that the user wants to add
    */
    void SetAnElementColourRGB(std::string AtomSymbol, std::vector<int>& RGBVector);

    /**
     * Returns dictionary of van der Waals radii of atoms
     * @return The specified dictionary, VDWRadii.
    */
    std::map<std::string, double> GetVDWRadii();

    /**
     * Sets a van der Waals radius for a specific atomic symbol in the VDWRadii dictionary.
     * @param AtomSymbol The symbol for the atom's element.
     * @param Radius The radius to set.
    */
    void SetVDWRadius(std::string AtomSymbol, double Radius);

    /**
     * Returns the radius scale to be put on each atom - used for relative enlargening/shrinking of atoms
     * @return RadiiScale - Radius scale variable
    */
    int GetRadiiScale();

    /**
     * Overwrites radius scale variable - useful for increasing/decreasing atoms at will.
     * @param NewScale The new value for the scale you'd like to set.
    */
    void SetRadiiScale(int NewScale);


private:
    /**
    * ElementColoursRGB - A dictionary containing RGB values for various element symbols.
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

    /**
     * VDWRadii - a dictionary containing hard-sphere-approximated radii of atoms (in nm). Some disagreements between literature values exist.
     * Taken from the averages of Tables 1, 2 and 7 in "Van der Waals Radii of Elements" (Batsanov, 2001).
    */
    std::map<std::string, double> VDWRadii = { {"H", 0.12}, {"C", 0.17}, {"N", 0.15}, {"O", 0.14}, {"S", 0.18}, {"P", 0.208}, {"Fe", 0.20}, {"Zn", 0.21} };

    /**
     * RadiiScale - a value corresponding to how large or small to make the atom spheres appear.
    */
    int RadiiScale{ 10 };
};
