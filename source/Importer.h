// by Subramanya N
// contact: subramanyanagabhushan@gmail.com

#pragma once
#include "Engine/StaticMesh.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Async.h"
#include "ThreadSafeBool.h"
#include "ImportedMesh.h"

#include "Importer.generated.h"


UCLASS()
class YourProject_API AImporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static int count;

	static FThreadSafeBool stopped;

	//Root Component
	USceneComponent* rootComponent;

	static void read3Dfile(FString path, FString filename, FString address, AImportedMesh* pmcActor, TArray<int> pFlags);

	UFUNCTION(BlueprintCallable, Category = "renderpub")
	AImportedMesh* read3DFiles(FString path, TArray<int> pFlag);

};

class BackgroundImporter : public FNonAbandonableTask
{
	FString path, filename, address;
	TArray<int> pFlags;
	AImportedMesh* pmcActor;
public:
	/*Default constructor*/
	BackgroundImporter(FString path, FString filename, FString address, AImportedMesh* pmcActor, TArray<int> pFlags)
	{
		this->path = path;
		this->filename = filename;
		this->address = address;
		this->pmcActor = pmcActor;
		this->pFlags = pFlags;
	}

	/*This function is needed from the API of the engine.
	My guess is that it provides necessary information
	about the thread that we occupy and the progress of our task*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(BackgroundImporter, STATGROUP_ThreadPoolAsyncTasks);
	}

	/*This function is executed when we tell our task to execute*/
	void DoWork()
	{
		AImporter::read3Dfile(path, filename, address, pmcActor, pFlags);

		GLog->Log("--------------------------------------------------------------------");
		GLog->Log("BackgroundImporter Stopped");
		GLog->Log("--------------------------------------------------------------------");
	}
};