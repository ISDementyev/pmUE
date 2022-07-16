//
//  Atom.hpp
//  pmUE
//
//  Created by Ilya Dementyev on 2022-07-15.
//

#pragma once

#include <iostream>

using std::string;

class Atom
{
public:
    Atom(string en, int id, string c, double r);
    
    string GetElementName();
    void SetElementName(string NewName);
    
    int GetID();
    void SetID(int NewID);
    
    string GetColour();
    void SetColour(string NewColour);
    
    double GetRadius();
    void SetRadius(double NewRadius);
    
private:
    string ElementName;
    int ID;
    string Colour;
    double Radius;
    
};


