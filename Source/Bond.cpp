#include <iostream>
#include "Bond.hpp"

/**
 * A Bond constructor
 */
Bond::Bond(char t, double l) : Type{ t }, Length{ l } {}

/**
 * Returns the type of bond
 * @return Bond type (single, double, etc.)
 */
char Bond::GetType()
{
    return Type;
}

/**
 * Sets the bond type
 */
void Bond::SetType(char NewType)
{
    if (NewType == 'S' || NewType == 'D'
        || NewType == 'T' || NewType == 'Q')
    {
        Type = NewType;
    }
}

/**
 * Returns bond length
 * @return Bond length (in nm)
 */
double Bond::GetLength()
{
    return Length;
}

/**
 * Sets new length
 */
void Bond::SetLength(double NewLength)
{
    if (NewLength > 0)
    {
        Length = NewLength;
    }
}
