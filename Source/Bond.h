#pragma once

class Bond
{
public:
    /**
     * A Bond constructor
     */
    Bond(char t, double l);
    
    /**
     * Returns the type of bond
     * @return Bond type (single, double, etc.)
     */
    char GetType();
    
    /**
     * Sets the bond type
     */
    void SetType(char NewType);
    
    /**
     * Returns bond length
     * @return Bond length (in nm)
     */
    double GetLength();
    
    /**
     * Sets new length
     */
    void SetLength(double NewLength);
    
private:
    char Type;
    double Length;
};
