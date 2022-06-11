/**
* A file containing important tools that will be used within the plugin
*/

#pragma once

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <iostream>
#include <fstream>

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
