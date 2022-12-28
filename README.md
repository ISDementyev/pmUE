# pmUE
pmUE (Protein Modelling Unreal Engine) - a repo for constructing a molecule visualizer plugin in Unreal

### Current abilities
The current capabilities are shown below for Methane.pdb and AceticAcid.pdb respectively. Appropriate bonds and bond lengths must still be implemented.

![Methane](/Screenshots/MethaneProperRadii.png "Spawned methane with color and proper radii")
![Acetic acid](/Screenshots/AceticAcidProperRadii.png "Spawned acetic acid with color and proper radii")

### Main Ideas
1. Unreal Engine 4 (mainly in C++) used alongside Visual Studio 2019 for the majority of the work, with Doxygen-style commenting (html file will be added in a docs folder)
2. pmUE will be more akin to an Unreal plugin than a standalone "game"

### Current Branch - ElementSpecificSpawner
This branch was designed to explore another possible implementation, that solves the issue with the improper scaling - to create a generator Actor for each element, and spawn the atoms as such, with the individual atom static meshes already setup (for both color and size). This branch has solved the issue using the strategies mentioned, so it will be pushed to ```main``` soon.

### Collaboration
To my knowledge, I am the only individual working on this repo. As the code is slowly being developed, it is recommended to open your own branch if you'd like to add anything to the project. All contributions welcome!

### Acknowledgement
If you use any of the code listed here (or the entire plugin) please say so in the acknowledgements section of your published media (e.g. journal, article, video, etc.).

A good template: "The molecular visualization was performed using pmUE (https://github.com/ISDementyev/pmUE)".

Of course, you may write this in whatever language is required for you to write in.

### Citations
Avogadro 1.2.0 was used for the creation of some models in [PDBFiles](https://github.com/ISDementyev/pmUE/tree/main/PDBFiles)
- Avogadro: an open-source molecular builder and visualization tool. Version 1.XX. http://avogadro.cc/
- Marcus D Hanwell, Donald E Curtis, David C Lonie, Tim Vandermeersch, Eva Zurek and Geoffrey R Hutchison; “Avogadro: An advanced semantic chemical editor, visualization, and analysis platform” [*Journal of Cheminformatics* **2012**, 4:17.](http://www.jcheminf.com/content/4/1/17)
