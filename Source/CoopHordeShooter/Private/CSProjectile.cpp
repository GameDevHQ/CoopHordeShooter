#include "CSProjectile.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

static int32 DebugProjectileDrawing = 0;
FAutoConsoleVariableRef CVARDebugProjectileDrawing (
    TEXT("COOP.DebugProjectiles"), 
    DebugProjectileDrawing, 
    TEXT("Draw debug spheres for projectiles"), 
    ECVF_Cheat
);


ACSProjectile::ACSProjectile():
Damage(25.0f),
DamageRadius(200.0f)
{
    // The default grenade launcher mesh
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetCollisionProfileName("Projectile");
    MeshComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    MeshComponent->CanCharacterStepUpOn = ECB_No;
    RootComponent = MeshComponent;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = MeshComponent;
    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->MaxSpeed = 2000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;

    // Destroy an object after 1 second
    InitialLifeSpan = 1.0f;
}

void ACSProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (ExplosionEffect)
    {
        GetWorldTimerManager().SetTimer(EffectTimerHandler, this, &ACSProjectile::PlayExplosionEffect, InitialLifeSpan);
    }
}

void ACSProjectile::PlayExplosionEffect()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator(0.0f));

    TSubclassOf<UDamageType> DamageType;
    TArray<AActor*> IgnoreActors;
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 
                                        DamageRadius, DamageType, IgnoreActors, 
                                        this, GetInstigatorController(), true);

    if (DebugProjectileDrawing > 0)
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 12, FColor::Yellow, false, 2.0f, 0, 2);
    }
    
    Destroy();
}
