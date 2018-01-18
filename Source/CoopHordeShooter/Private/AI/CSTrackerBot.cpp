#include "CSTrackerBot.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


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

FVector ACSTrackerBot::GetNextPathPoint()
{
    ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

    // Try to build a path to a player and return the first available from collection
    UNavigationPath* NavigationPath = UNavigationSystem::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn); 
    if (NavigationPath->PathPoints.Num() > 1)
    {
        return NavigationPath->PathPoints[1];
    }

    // Path to a player doesn't exist. Return the current actor location
    return GetActorLocation();
}

// Called every frame
void ACSTrackerBot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
