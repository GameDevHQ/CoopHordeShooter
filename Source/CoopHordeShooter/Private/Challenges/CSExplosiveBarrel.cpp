#include "CSExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"
#include "CSHealthComponent.h"


// Sets default values
ACSExplosiveBarrel::ACSExplosiveBarrel():
bIsExploded(false),
ExplosionForce(100.0f)
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
    RootComponent = MeshComponent;

    HealthComponent = CreateDefaultSubobject<UCSHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->OnHealthChanged.AddDynamic(this, &ACSExplosiveBarrel::OnHealthChanged);

    RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
    RadialForceComponent->bImpulseVelChange = true;
    RadialForceComponent->bAutoActivate = true;
    RadialForceComponent->SetupAttachment(RootComponent);

    SetReplicates(true);
    SetReplicateMovement(true);
    MinNetUpdateFrequency = 20.0f;
}

void ACSExplosiveBarrel::OnRep_Exploded()
{
    PlayExplosionEffects();
}

void ACSExplosiveBarrel::OnHealthChanged(UCSHealthComponent* HealthComp, float Health,
                                         float HealthDelta, const UDamageType* DamageType, 
                                         AController* InstigatedBy, AActor* DamageCauser)
{
    if (bIsExploded)
    {
        return;
    }

    if (Health <= 0.0f)
    {
        bIsExploded = true;

        // Push an object up
        FVector BoostIntensity = FVector::UpVector * ExplosionForce;
        MeshComponent->AddImpulse(BoostIntensity, NAME_None, true);

        OnRep_Exploded();

        // Blast away nearby physics actors
        RadialForceComponent->FireImpulse();
    }
}

void ACSExplosiveBarrel::PlayExplosionEffects()
{
    // Play a visual effect and change the object material to an another one
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
    MeshComponent->SetMaterial(0, ExplodedMaterial);
}

void ACSExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSExplosiveBarrel, bIsExploded);
}