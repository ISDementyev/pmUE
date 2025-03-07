# pmUE
pmUE (Protein Modelling Unreal Engine) - a repo for constructing a molecule visualizer plugin in Unreal.

Main contributor(s): Ilya Sergeevich Dementyev

### Most Recent Update
This project will be released as a pre-release soon.

### Current abilities
The current capabilities are shown below, for a small peptide seq: IKK. Appropriate bonds and bond lengths must still be implemented.

![6A5J IKK Peptide](/Screenshots/6A5JSmallIKK.png "Spawned IKK peptide from 6A5J, smaller HUD for future edits")

### How to use
First, open up any UE4 version. To be safe, you may use the same one I did, `4.27.2` on Windows 10.

Let's say your current C++ UE4 session/project name is `YourProjectName`. Any UE4 project has an associated `YourProjectName.Build.cs` file, that appears as the following:
```csharp
using UnrealBuildTool;

public class YourProjectName : ModuleRules
{
    public YourProjectName(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
```
After you've downloaded the source code, you'll want to add the following:
```csharp
using UnrealBuildTool;
using System.IO;

public class YourProjectName : ModuleRules
{
    public YourProjectName(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicSystemIncludePaths.AddRange(new string[] { "PathToSource/Source" });
    }
}
```
Making sure to do this for all of the folders. 

Blueprints should be added to `YourProjectName/Content/YourProjectName/Blueprints`, Static Meshes to `YourProjectName/Content/YourProjectName/Art/StaticMeshes`, Materials to `YourProjectName/Content/YourProjectName/Art/Materials`

For the file that you want to visualize, you must first copy it to `YourProjectName/Source/YourProjectName/`, then write the name in the `FString FileName{ "filename.pdb" };` line in `FileData.h`. Currently, only PDB files are supported.

Once all of this is done, you must rebuild your UE4 C++ project `YourProjectName`.

If everything goes well, this should have not broken anything. Continuing on, load all blueprints into the Game World, and center them all at 0,0,0.

Then, upon pressing Play, your molecule should render as spheres, with the PDB file's centroid @ origin.

### Main Ideas
1. Unreal Engine 4 (mainly in C++) used alongside Visual Studio 2019 for the majority of the work, with Doxygen-style commenting (html file will be added in a docs folder)
2. pmUE will be more akin to an Unreal plugin than a standalone "game"
3. If larger molecules will significantly slow down the viewer, then Nanite will be used.  

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

The Protein Data Bank (PDB) was used for extracting of small peptides like 6A5J
- The Protein Data Bank H.M. Berman, J. Westbrook, Z. Feng, G. Gilliland, T.N. Bhat, H. Weissig, I.N. Shindyalov, P.E. Bourne (2000) Nucleic Acids Research, 28: 235-242. [https://doi.org/10.1093/nar/28.1.235](https://doi.org/10.1093/nar/28.1.235)
