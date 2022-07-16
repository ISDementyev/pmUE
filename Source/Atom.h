#pragma once

#include <iostream>

using std::string;

class Atom
{
public:
    /**
     * An atom constructor
     */
    Atom(string en, int id, string c, double r);
    
    /**
     * Returns the atom's element
     * @return Name of the element
     */
    string GetElementName();
    
    /**
     * Changes the atom's element
     */
    void SetElementName(string NewName);
    
    /**
     * Returns the atom's ID number (in order that can be found in the pdb file)
     * @return Atom's ID number
     */
    int GetID();
    
    /**
     * Gives the atom a new ID
     */
    void SetID(int NewID);
    
    
    /**
     * Returns the atom's colour
     * @return The atom's colour
     */
    string GetColour();
    
    /**
     * Sets the colour for the atom
     */
    void SetColour(string NewColour);
    
    /**
     * Returns atom's radius
     * @return Atom's radius (in nm)
     */
    double GetRadius();
    
    /**
     * Sets atom's radius
     */
    void SetRadius(double NewRadius);
    
private:
    string ElementName;
    int ID;
    string Colour;
    double Radius;
    
};


