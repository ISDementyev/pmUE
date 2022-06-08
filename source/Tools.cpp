/**
* A file containing important tools that will be used within the plugin
*/

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

/**
* Removes all hydrogens
*/
void AtomManager::RemoveHydrogens()
{
	// find all hydrogen atoms and collect them in an array
	TArray<AActor*> AllHydrogens;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "AtomH", AllHydrogens);
  
  if (AllHydrogens.Num() > 0)
  {
    // destroy (remove) all hydrogen atoms in the "game"
    for (int i = 0; i < AllHydrogens.Num(); i++)
    {
      AllHydrogens[i]->Destroy();
    }
  }
}
