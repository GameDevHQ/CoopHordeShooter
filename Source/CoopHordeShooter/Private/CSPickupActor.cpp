#include "CSPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
ACSPickupActor::ACSPickupActor()
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetSphereRadius(75.0f);
    RootComponent = SphereComponent;

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->DecalSize = FVector(64.0f, 75.0f, 75.0f);
    DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    DecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACSPickupActor::BeginPlay()
{
    Super::BeginPlay();
}

void ACSPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
}
