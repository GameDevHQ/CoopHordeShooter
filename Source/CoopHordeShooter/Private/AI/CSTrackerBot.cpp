#include "CSTrackerBot.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ACSTrackerBot::ACSTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetCanEverAffectNavigation(false);
    RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ACSTrackerBot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACSTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
