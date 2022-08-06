// See GPL-v3.0 LICENSE in GitHub repo.


#include "GenerateAtomsAndBonds.h"
#include "Tools.h"

// Sets default values
AGenerateAtomsAndBonds::AGenerateAtomsAndBonds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	
}

// Called when the game starts or when spawned
void AGenerateAtomsAndBonds::BeginPlay()
{
	Super::BeginPlay();
		
	SetRootComponent(InstancedStaticMeshComponent);

	// Immobilize the atoms - we don't want them to move accidentally
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);

	// Turn off collisions and overlap events (don't need them)
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);

	int NAtoms = 8; // Tools::NumberOfAtoms("AceticAcid.pdb", false); // 8
	auto Coordinates = Tools::AcquireCoordinates("AceticAcid.pdb", false); // auto type used for code readability

	if (InstancedStaticMeshComponent->GetInstanceCount() == 0) // if they're already present, don't make more
	{
		TArray<FTransform> Transforms;
		Transforms.Empty(NAtoms);

		for (int i = 0; i < NAtoms; i++)
		{
			double X = std::get<1>(Coordinates).at(0).at(0);
			double Y = std::get<1>(Coordinates).at(0).at(1);
			double Z = std::get<1>(Coordinates).at(0).at(2);

			Transforms.Add(FTransform(FVector(X, Y, Z)));
		}

		InstancedStaticMeshComponent->AddInstances(Transforms, false); // will add same-size white spherical atoms to all coordinates
		// next, we have to change the size and colour of all instanced atoms present
	}

}

// Called every frame
void AGenerateAtomsAndBonds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

