// by Subramanya N
// contact: subramanyanagabhushan@gmail.com


#include "Importer.h"


int AImporter::count;

FThreadSafeBool AImporter::stopped;

// Sets default values
AImporter::AImporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(rootComponent);
}

// Called when the game starts or when spawned
void AImporter::BeginPlay()
{
	Super::BeginPlay();
	stopped.AtomicSet(false);
}

void AImporter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	stopped.AtomicSet(true);
}

// Called every frame
void AImporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AImporter::read3Dfile(FString path, FString filename, FString address, AImportedMesh* pmcActor, TArray<int> pFlags) {
	Assimp::Importer importer;
	//aiProcess_FlipUVs
	int propertiesFlags =  aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType ;
	for (auto tempFlag : pFlags) {
		switch (tempFlag)
		{
			//Flip UV
		case 0:
			propertiesFlags += aiProcess_FlipUVs;
			break;
			//MakeleftHanded
		case 1:
			propertiesFlags += aiProcess_MakeLeftHanded;
			break;
			//Flip winding order
		case 2:
			propertiesFlags += aiProcess_FlipWindingOrder;
			break;
		}
		
	}
	const aiScene* scene = importer.ReadFile(TCHAR_TO_UTF8(*path), propertiesFlags);
	
	const int32 NumSections = (int32)scene->mNumMeshes;
	TArray<int> MaterialRemap;
	MaterialRemap.Reserve(NumSections);

	for (int i = 0; i < (int)scene->mNumMeshes; i++) {
		TArray<FVector> Vertices;
		TArray<FVector> Normals;
		TArray<FVector2D> TextureCoordinates;
		TArray<int> Triangles;
		TArray<FRuntimeMeshTangent> Tangents;
		TArray<FLinearColor> VertexColors;
		int SectionIndex = 0 ;
		FString diffusePath = "", normalPath = "", metalnessPath ="";
		FColor fcolor = FColor(1,1,1,1);

		auto mesh = scene->mMeshes[i];
		int trianglesCount = 0;
		for (int j = 0; j < (int)mesh->mNumFaces; j++) {
			auto face = mesh->mFaces[j];
			trianglesCount += mesh->mNumFaces;
		}
		
		Vertices.Reserve((int)mesh->mNumVertices);
		Triangles.Reserve(trianglesCount);
		Normals.Reserve((int)mesh->mNumVertices);
		TextureCoordinates.Reserve((int)mesh->mNumVertices);
		Tangents.Reserve((int)mesh->mNumVertices);
		VertexColors.Reserve((int)mesh->mNumVertices);
		
		auto materialInstance = scene->mMaterials[mesh->mMaterialIndex];

		if (materialInstance->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString tempTexturePath;
			FString TempTexturePath;
			materialInstance->GetTexture(aiTextureType_DIFFUSE, 0, &tempTexturePath);

			TempTexturePath = FString(tempTexturePath.C_Str());

			FString n1, n2, n3, n4;
			bool splitRes1 = TempTexturePath.Split("\\", &n1, &n2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			bool splitRes2 = n1.Split("\\", &n3, &n4, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			diffusePath = address + "/" + n4 + "/" + n2;
		}
		else {
			aiColor3D color;
			if (AI_SUCCESS == materialInstance->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
				fcolor = FColor((color.r * 255), (color.g * 255), (color.b * 255), 255.0f);
				
			}
		}
		if (materialInstance->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString tempTexturePath;
			FString TempTexturePath;
			materialInstance->GetTexture(aiTextureType_NORMALS, 0, &tempTexturePath);

			TempTexturePath = FString(tempTexturePath.C_Str());

			FString n1, n2, n3, n4;
			bool splitRes1 = TempTexturePath.Split("\\", &n1, &n2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			bool splitRes2 = n1.Split("\\", &n3, &n4, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			normalPath = address + "/" + n4 + "/" + n2;
		}
		if (materialInstance->GetTextureCount(aiTextureType_METALNESS) > 0) {
			
			aiString tempTexturePath;
			FString TempTexturePath;
			materialInstance->GetTexture(aiTextureType_METALNESS, 0, &tempTexturePath);

			TempTexturePath = FString(tempTexturePath.C_Str());

			FString n1, n2, n3, n4;
			bool splitRes1 = TempTexturePath.Split("\\", &n1, &n2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			bool splitRes2 = n1.Split("\\", &n3, &n4, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			metalnessPath = address + "/" + n4 + "/" + n2;
		}

		//Create the vertex
		int32 NumVertex = mesh->mNumVertices;
		TMap<int32, FVector> VertexPositions;
		VertexPositions.Reserve(NumVertex);
		for (int32 VertexIndex = 0; VertexIndex < NumVertex; ++VertexIndex)
		{
			VertexPositions.Add(VertexIndex ,FVector(mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].z));
			Vertices.Push(FVector(mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].z));
			Normals.Push(FVector(mesh->mNormals[VertexIndex].y, mesh->mNormals[VertexIndex].x, mesh->mNormals[VertexIndex].z));
			Tangents.Push(FRuntimeMeshTangent(mesh->mTangents[VertexIndex].y, mesh->mTangents[VertexIndex].x, mesh->mTangents[VertexIndex].z));
			
			if ((int)mesh->GetNumColorChannels() > 0) {
				VertexColors.Push(FLinearColor(mesh->mColors[0][VertexIndex].r, mesh->mColors[0][VertexIndex].g, mesh->mColors[0][VertexIndex].b, mesh->mColors[0][VertexIndex].a));
			}
			else {
				VertexColors.Push(FLinearColor::White);
			}
			TextureCoordinates.Push(FVector2D(mesh->mTextureCoords[0][VertexIndex].x, mesh->mTextureCoords[0][VertexIndex].y));
		}
		for (int j = 0; j < (int)mesh->mNumFaces; j++) {
			auto face = mesh->mFaces[j];
			for (int32 index = 0; index < (int)face.mNumIndices; index++) {
				Triangles.Push(face.mIndices[index]);
			}
		}

		//Create Component in actor ImportedMesh
		AsyncTask(ENamedThreads::GameThread, [=]() {
			pmcActor->drawSection(FString::FromInt(i), Vertices, Normals, TextureCoordinates, Triangles, Tangents, VertexColors, 0, fcolor, diffusePath, normalPath, metalnessPath);
		});
	}
	pmcActor->setComplete();
}

AImportedMesh* AImporter::read3DFiles(FString path, TArray<int> pFlags) {
	FString name, address, fileType, filename;
	bool splitRes = path.Split("/", &address, &name, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (splitRes) {
		name = name;
	}
	else {
		name = path;
	}
	splitRes = name.Split(".", &filename, &fileType, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FVector defaultSpawnLocation(0.0f, 0.0f, 0.0f);
	FRotator defaultSpawnRotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	FString tempName = FString::FromInt(count) + filename;
	SpawnInfo.Name = FName(*tempName);
	count += 1;

	AImportedMesh* pmcActor = GetWorld()->SpawnActor<AImportedMesh>(defaultSpawnLocation, defaultSpawnRotation, SpawnInfo);

	(new FAutoDeleteAsyncTask<BackgroundImporter>( path, filename, address, pmcActor, pFlags))->StartBackgroundTask();

	return pmcActor;
}