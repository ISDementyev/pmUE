// See GPL-v3.0 LICENSE.MD in GitHub repo.


#include "AtomGeneratorOne.h"
#include "ToolsFunctionLibrary.h"

/**
 * Sets default values
*/
AAtomGeneratorOne::AAtomGeneratorOne()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));

	RootComponent = InstancedStaticMeshComponent;

	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);

}

/**
 * Constructs the atom instances in AtomGeneratorOne blueprint
 * @param Transform The locations (position vectors) at which to spawn the instances (atoms)
*/
void AAtomGeneratorOne::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//UE_LOG(LogTemp, Warning, TEXT("Beginning..."));

	//FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
	//int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtoms(PDBContent);

	//Transforms.Empty(NumberOfAtoms);

	//FVector CentroidCoord = UToolsFunctionLibrary::Centroid(PDBContent, false);

	////UToolsFunctionLibrary::CentroidCorrected(PDBContent, Transforms, CentroidCoord, false);

	//UToolsFunctionLibrary::GetCoordinates(PDBContent, Transforms, false);

	//InstancedStaticMeshComponent->AddInstances(Transforms, true);
	//InstancedStaticMeshComponent->SetCustomDataValue(0, 0, 1.0f, true);
}

/**
 * Called when the game starts or when spawned
*/
void AAtomGeneratorOne::BeginPlay()
{
	Super::BeginPlay();

	FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtoms(PDBContent, false);

	Transforms.Empty(NumberOfAtoms);

	FVector CentroidCoord = UToolsFunctionLibrary::Centroid(PDBContent, false);

	UToolsFunctionLibrary::CentroidCorrected(PDBContent, Transforms, CentroidCoord, false);

	InstancedStaticMeshComponent->AddInstances(Transforms, true);

	TMap<int32, FString> IndicesAndAtomNames = UToolsFunctionLibrary::AtomNameAndIndex(PDBContent, false);
	int32 TotalIndices{ NumberOfAtoms * 3 };

	for (int32 i = 0; i < NumberOfAtoms; i++)
	{
		FString ElementSymbol = IndicesAndAtomNames[i];
		FVector Color = UToolsFunctionLibrary::GetElementColourRGB(ElementSymbol);

		for (int32 j = 0; j < 3; j++)
		{
			InstancedStaticMeshComponent->SetCustomDataValue(i, j, Color[j], true);
		}
	}	

	// debugging
// 	for (int32 i = 0; i < IndicesAndAtomNames.Num(); i++)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("@ Map index %d we have: '%s'"), i, *IndicesAndAtomNames[i]);
// 	}

	// debugging
	//FString Atom = IndicesAndAtomNames[1]; // all debugging code works with this input. where is the error coming from?
	//FVector Color = UToolsFunctionLibrary::GetElementColourRGB(Atom);
	//UE_LOG(LogTemp, Warning, TEXT("Color index 0 of white RGB: %f"), Color[0]);

}

/**
 * Called every frame
 * @param DeltaTime The in-game time variable
*/
void AAtomGeneratorOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

