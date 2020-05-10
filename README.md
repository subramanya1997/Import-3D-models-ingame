# Import 3D models ingame using Procedural Mesh and Assimp : UE4
 3D file importer to UE4 project at runtime using Assimp and Procedural Mesh

#Assimp
Open Asset Import Library (short name: Assimp) is a portable Open Source library to import various well-known 3D model formats in a uniform manner. The most recent version also knows how to export 3d files and is therefore suitable as a general-purpose 3D model converter.
Asset-Importer-Lib is ...
1. Written in portable, ISO-compliant C++ ( C++11 supported )
2. Easily configurable and customizable build chain via cmake
3. Core interface / API is provided for both C++ and C
4. Easy to configure Post-processing pipeline
5. Command-line interface to perform common operations (i.e. quick file stats, convert models, extract embedded textures) from the shell
6. Imports bones, vertex weights and animations (i.e. skinning, skeletal animations)
7. Loads multiple UV and vertex color channels (current limit is 8)
8. Works well with UNICODE input files
9. Supports complex multi-layer materials
10. Supports embedded textures, both compressed (e.g. PNG) or just raw color data

#Procedural Mesh
Its an experemental plugin of unreal engine.

#Steps to follow:
1. assimp-vc141-mt.dll should be placed in Binaries of your project Binaries.
2. thirdparty folder should be placed in your project folder
3. Source has importer and importedMesh should be placed in your source folder and importer needs to be spawned in the game.
4. Add in your MyProject.Build.cs:
	PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent" });
5. Add in your MyProject.Build.cs:
	var basePath = Path.GetDirectoryName(RulesCompiler.GetFileNameFromType(GetType()));
	string thirdPartyPath = Path.Combine(basePath, "..", "..", "Thirdparty");
6.  Add in you MyProject.Build.cs:
	PublicIncludePaths.Add(Path.Combine(thirdPartyPath, "assimp", "include"));
	PublicLibraryPaths.Add(Path.Combine(thirdPartyPath, "assimp", "lib"));
	PublicAdditionalLibraries.Add("assimp-vc141-mt.lib");
