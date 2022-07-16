#include "Atom.h"

/**
 * An atom constructor
 */
Atom::Atom(string en, int id, string c, double r) : ElementName{ en }, ID { id }, Colour{ c }, Radius{ r } {}

/**
 * Returns the atom's element
 * @return Name of the element
 */
string Atom::GetElementName()
{
    return ElementName;
}

/**
 * Changes the atom's element
 */
void Atom::SetElementName(string NewName)
{
    ElementName = NewName;
}

/**
 * Returns the atom's ID number (in order that can be found in the pdb file)
 * @return Atom's ID number
 */
int Atom::GetID()
{
    return ID;
}

/**
 * Gives the atom a new ID
 */
void Atom::SetID(int NewID)
{
    ID = NewID;
}

/**
 * Returns the atom's colour
 * @return The atom's colour
 */
string Atom::GetColour()
{
    return Colour;
}

/**
 * Sets the colour for the atom
 */
void Atom::SetColour(string NewColour)
{
    Colour = NewColour;
}

/**
 * Returns atom's radius
 * @return Atom's radius (in nm)
 */
double Atom::GetRadius()
{
    return Radius;
}

/**
 * Sets atom's radius
 */
void Atom::SetRadius(double NewRadius)
{
    Radius = NewRadius;
}
