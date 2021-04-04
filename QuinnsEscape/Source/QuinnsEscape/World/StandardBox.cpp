


#include "StandardBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AStandardBox::AStandardBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> boxObject(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	if (boxObject.Succeeded())
	{
		BoxMesh->SetStaticMesh(boxObject.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find box!"));
		return;
	}
}

// Called when the game starts or when spawned
void AStandardBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStandardBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

