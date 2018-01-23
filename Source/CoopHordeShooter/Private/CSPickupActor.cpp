#include "CSPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "CSPowerupActor.h"
#include "CSCharacter.h"


// Sets default values
ACSPickupActor::ACSPickupActor():
RespawnTime(10.0f)
{
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetSphereRadius(75.0f);
    RootComponent = SphereComponent;

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->DecalSize = FVector(64.0f, 75.0f, 75.0f);
    DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    DecalComponent->SetupAttachment(RootComponent);

    SetReplicates(true);
}

// Called when the game starts or when spawned
void ACSPickupActor::BeginPlay()
{
    Super::BeginPlay();

    if (Role == ROLE_Authority)
    {
        Respawn();   
    }
}

void ACSPickupActor::Respawn()
{
    if (PowerUpClass == nullptr)
    { 
        UE_LOG(LogTemp, Warning, TEXT("PowerUpClass is nullptr. Please update your blueprint."), *GetName());
        return;
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PowerUpInstance = GetWorld()->SpawnActor<ACSPowerupActor>(PowerUpClass, GetTransform(), SpawnParameters);  
}

void ACSPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    ACSCharacter* Player = Cast<ACSCharacter>(OtherActor);
    if (Role == ROLE_Authority && PowerUpInstance && Player)
    {
        PowerUpInstance->ActivatePowerup(OtherActor);
        PowerUpInstance = nullptr;

        GetWorldTimerManager().SetTimer(TimerHandle_RespawnPowerUp, this, &ACSPickupActor::Respawn, RespawnTime, false);
    }
}
