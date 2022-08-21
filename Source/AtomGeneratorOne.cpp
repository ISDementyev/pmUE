// See GPL-v3.0 LICENSE.MD in GitHub repo.


#include "AtomGeneratorOne.h"
#include "ToolsFunctionLibrary.h"

// Sets default values
AAtomGeneratorOne::AAtomGeneratorOne()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	
	RootComponent = InstancedStaticMeshComponent;
	
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);

}

void AAtomGeneratorOne::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

// 	FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
// 	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtoms(PDBContent);

	/*if (InstancedStaticMeshComponent->GetInstanceCount() == 0)
	{
		Transforms.Empty(NumberOfAtoms);
		
		UToolsFunctionLibrary::GetCoordinates(PDBContent, Transforms, false);

		InstancedStaticMeshComponent->AddInstances(Transforms, false);
	}*/
}

// Called when the game starts or when spawned
void AAtomGeneratorOne::BeginPlay()
{
	Super::BeginPlay();
	
	FString PDBContent = UToolsFunctionLibrary::ConvFileToString("Methane.pdb");
	int32 NumberOfAtoms = UToolsFunctionLibrary::NumberOfAtoms(PDBContent);

	Transforms.Empty(NumberOfAtoms);
	UToolsFunctionLibrary::GetCoordinates(PDBContent, Transforms, false);

	InstancedStaticMeshComponent->AddInstances(Transforms, true);
}

// Called every frame
void AAtomGeneratorOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

