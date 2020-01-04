// by Subramanya N
// contact: subramanyanagabhushan@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RuntimeMeshComponent.h"
#include "RuntimeMeshLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Materials.h"

#include "ImportedMesh.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FImporteComplete);

UCLASS()
class YourProject_API AImportedMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImportedMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Root component
	USceneComponent* rootComponent;

	//Components Map
	TMap<FString, URuntimeMeshComponent*> proceduralMeshComponents;
	//Material Map
	TMap<FString, UMaterialInstanceDynamic*> matDict;

	UMaterial* masterMaterial;
	UMaterialInstanceDynamic* defaultMaterialInstance;

	UPROPERTY(BlueprintAssignable, Category = Utility)
		FImporteComplete onComplete;

	//Add component
	void createComponent(FString componentName);

	//Draw Section
	void drawSection(FString componentName, TArray<FVector> Vertices, TArray<FVector> Normals, TArray<FVector2D> TextureCoordinates, TArray<int> Triangles, TArray<FRuntimeMeshTangent> Tangents, TArray<FLinearColor> VertexColors, int SectionIndex, FColor color, FString diffusePath, FString normalPath, FString metalnessPath);
	
	//Complete
	void setComplete();
};
