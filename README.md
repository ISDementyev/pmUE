# pmUE
pmUE (Protein Modelling Unreal Engine) - a repo for constructing a molecule visualizer plugin in Unreal

### Current State
Currently, the project can spawn atoms in-game, with the AtomConstructorOne class blueprint. The current capabilities are shown below for Methane.pdb and AceticAcid.pdb respectively. Appropriate bond lengths as well as atom colours and radii must still be implemented.

![Methane](https://github.com/ISDementyev/pmUE/blob/main/Screenshots/SpawnedMethane.png "Spawned Methane")
![AceticAcid](https://github.com/ISDementyev/pmUE/blob/main/Screenshots/SpawnedAceticAcid.png "Spawned Acetic Acid")

A centroid function will be added soon that can spawn the atoms more towards the center of where the blueprint was placed.

### Main Ideas
1. Will use Unreal Engine 4 (with C++) alongside Visual Studio 2019 for the majority of the work, with Doxygen-style commenting (html file will be added in a docs folder)
2. pmUE will be more akin to an Unreal plugin than a standalone "game"
3. First the code will be designed standalone; it will then be edited into a format readable by the UE
4. Develop centroid function (see above)

### Collaboration
As the code is slowly being developed, it is recommended to open your own branch if you'd like to add anything to the project. All contributions welcome!

### Acknowledgement
If you use any of the code listed here (or the entire plugin) please say so in the acknowledgements section of your published media (e.g. journal, article, video, etc.).

A good template: "The molecular visualization was performed using pmUE (https://github.com/ISDementyev/pmUE)".

Of course, you may write this in whatever language is required for you to write in.

### Citations
Avogadro 1.2.0 was used for the creation of some models in [PDBFiles](https://github.com/ISDementyev/pmUE/tree/main/PDBFiles)
- Avogadro: an open-source molecular builder and visualization tool. Version 1.XX. http://avogadro.cc/
- Marcus D Hanwell, Donald E Curtis, David C Lonie, Tim Vandermeersch, Eva Zurek and Geoffrey R Hutchison; “Avogadro: An advanced semantic chemical editor, visualization, and analysis platform” [*Journal of Cheminformatics* **2012**, 4:17.](http://www.jcheminf.com/content/4/1/17)
