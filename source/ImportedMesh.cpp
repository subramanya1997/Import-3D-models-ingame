// by Subramanya N
// contact: subramanyanagabhushan@gmail.com


#include "ImportedMesh.h"

FString mastermaterialPath = "/Game/Livesnc_master.Livesnc_master";

// Sets default values
AImportedMesh::AImportedMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(rootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> material(*mastermaterialPath);
	if (material.Succeeded()) {
		masterMaterial = material.Object;
	}
	else {
		//Handle fail case
	}
}

// Called when the game starts or when spawned
void AImportedMesh::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AImportedMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//---------------------------------------------------------------------
//                         createComponent
//---------------------------------------------------------------------

void AImportedMesh::createComponent(FString componentName) {
	GLog->Log("create");
	URuntimeMeshComponent* PMeshComponent = NewObject<URuntimeMeshComponent>(this, FName(*componentName));
	PMeshComponent->SetupAttachment(this->GetRootComponent());
	PMeshComponent->RegisterComponent();
	PMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	proceduralMeshComponents.Add(componentName, PMeshComponent);
}

//---------------------------------------------------------------------
//                         Draw Section
//---------------------------------------------------------------------

void AImportedMesh::drawSection(FString componentName, TArray<FVector> Vertices, TArray<FVector> Normals, TArray<FVector2D> TextureCoordinates, TArray<int> Triangles, TArray<FRuntimeMeshTangent> Tangents, TArray<FLinearColor> VertexColors, int SectionIndex, FColor color, FString diffusePath, FString normalPath, FString metalnessPath) {
	createComponent(componentName);
	//proceduralMeshComponents[componentName]->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, TextureCoordinates, TArray<FColor>(), Tangents, true, EUpdateFrequency::Infrequent, ESectionUpdateFlags::None, true, true);
	proceduralMeshComponents[componentName]->CreateMeshSection_Blueprint(SectionIndex, Vertices, Triangles, Normals, Tangents, TextureCoordinates, TArray<FVector2D>(), VertexColors, true, false, false, false, EUpdateFrequency::Average, false, true);

	
	UMaterialInstanceDynamic* sectionMaterial = UMaterialInstanceDynamic::Create(masterMaterial, nullptr);

	if (diffusePath.Len() != 0) {
		//Default JPG
		EJoyImageFormats extension = EJoyImageFormats::JPG;
		if (diffusePath.EndsWith(FString(".png"))) {
			extension = EJoyImageFormats::PNG;
		}
		else if ((diffusePath.EndsWith(FString(".jpg"))) || (diffusePath.EndsWith(FString(".jpeg")))) {
			extension = EJoyImageFormats::JPG;
		}
		else if (diffusePath.EndsWith(FString(".ico"))) {
			extension = EJoyImageFormats::ICO;
		}
		else if (diffusePath.EndsWith(FString(".bmp"))) {
			extension = EJoyImageFormats::BMP;
		}
		else if (diffusePath.EndsWith(FString(".icns"))) {
			extension = EJoyImageFormats::ICNS;
		}
		else if (diffusePath.EndsWith(FString(".exr"))) {
			extension = EJoyImageFormats::EXR;
		}
		else {
			//hnadle invalid case
		}
		bool isValid;
		int32 texWidth, texHeight;
		UTexture2D* textureParam = loadTexture2dFromFile(diffusePath, extension, isValid, texWidth, texHeight);
		sectionMaterial->SetTextureParameterValue("basecolor", textureParam);
		sectionMaterial->SetScalarParameterValue("trueColor", 0.0);
	}
	else {
		sectionMaterial->SetScalarParameterValue("trueColor", 1.0);
		sectionMaterial->SetVectorParameterValue("tint", color);
	}
	if (normalPath.Len() != 0) {
		//Default JPG
		EJoyImageFormats extension = EJoyImageFormats::JPG;
		if (normalPath.EndsWith(FString(".png"))) {
			extension = EJoyImageFormats::PNG;
		}
		else if ((normalPath.EndsWith(FString(".jpg"))) || (normalPath.EndsWith(FString(".jpeg")))) {
			extension = EJoyImageFormats::JPG;
		}
		else if (normalPath.EndsWith(FString(".ico"))) {
			extension = EJoyImageFormats::ICO;
		}
		else if (normalPath.EndsWith(FString(".bmp"))) {
			extension = EJoyImageFormats::BMP;
		}
		else if (normalPath.EndsWith(FString(".icns"))) {
			extension = EJoyImageFormats::ICNS;
		}
		else if (normalPath.EndsWith(FString(".exr"))) {
			extension = EJoyImageFormats::EXR;
		}
		else {
			//hnadle invalid case
		}
		bool isValid;
		int32 texWidth, texHeight;
		UTexture2D* textureParam = loadTexture2dFromFile(normalPath, extension, isValid, texWidth, texHeight);
		//Set Normal Map
	}
	if (metalnessPath.Len() != 0) {
		//Default JPG
		EJoyImageFormats extension = EJoyImageFormats::JPG;
		if (metalnessPath.EndsWith(FString(".png"))) {
			extension = EJoyImageFormats::PNG;
		}
		else if ((metalnessPath.EndsWith(FString(".jpg"))) || (metalnessPath.EndsWith(FString(".jpeg")))) {
			extension = EJoyImageFormats::JPG;
		}
		else if (metalnessPath.EndsWith(FString(".ico"))) {
			extension = EJoyImageFormats::ICO;
		}
		else if (metalnessPath.EndsWith(FString(".bmp"))) {
			extension = EJoyImageFormats::BMP;
		}
		else if (metalnessPath.EndsWith(FString(".icns"))) {
			extension = EJoyImageFormats::ICNS;
		}
		else if (metalnessPath.EndsWith(FString(".exr"))) {
			extension = EJoyImageFormats::EXR;
		}
		else {
			//hnadle invalid case
		}
		bool isValid;
		int32 texWidth, texHeight;
		UTexture2D* textureParam = loadTexture2dFromFile(metalnessPath, extension, isValid, texWidth, texHeight);
		//Set Normal Map
	}

	proceduralMeshComponents[componentName]->SetSectionMaterial(SectionIndex, sectionMaterial);
	proceduralMeshComponents[componentName]->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(1, 1, 1)));
}

void AImportedMesh::setComplete() {
	onComplete.Broadcast();
}