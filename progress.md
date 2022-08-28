## Progress

Currently, the project can spawn atoms in-game, with the AtomConstructorOne class blueprint. The current capabilities are shown below for Methane.pdb and AceticAcid.pdb respectively. Appropriate bond lengths as well as atom colours and radii must still be implemented.

![Methane](https://github.com/ISDementyev/pmUE/blob/main/Screenshots/SpawnedMethane.png "Spawned Methane")
![AceticAcid](https://github.com/ISDementyev/pmUE/blob/main/Screenshots/SpawnedAceticAcid.png "Spawned Acetic Acid")

A centroid function was added that spawns the atoms more towards the center of where the blueprint was placed.

Recently, a HUD was developed as well, that so far shows the filename only (shown below for Methane.pdb). In the future, more details about the loaded file will be written.
![HUD](https://github.com/ISDementyev/pmUE/blob/main/Screenshots/methane-with-hud.png "Methane with HUD")

Color is being developed as well, proper colors will incorporate UToolsFunctionLibrary::GetElementColourRGB.
![Color](https://github.com/ISDementyev/pmUE/blob/main/Screenshots/false-color.png "Methane - False color")
