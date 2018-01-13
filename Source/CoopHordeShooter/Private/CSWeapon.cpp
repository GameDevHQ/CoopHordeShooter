#include "CSWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopHordeShooter.h"
#include "TimerManager.h"


static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing (
    TEXT("COOP.DebugWeapons"), 
    DebugWeaponDrawing, 
    TEXT("Draw debug lines for weapons"), 
    ECVF_Cheat
);


// Sets default values
ACSWeapon::ACSWeapon():
MuzzleSocketName("MuzzleSocket"),
TracerTargetName("BeamEnd"),
BaseDamage(20.0f),
CriticalHitDamageMultiplier(2.50f),
FireRate(450.0f),
LastFiredTime(0.0f),
TimeBetweenShots(0.0f)
{
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ACSWeapon::Fire()
{
    // Trace the world from the pawn eyes to crosshair location
    AActor* Owner = GetOwner();
    if (Owner)
    {
        FVector EyeLocation;
        FRotator EyeRotation;
        Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

        FVector ShotDirection = EyeRotation.Vector();
        FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

        // The particle "Target" parameter
        FVector TracerEndPoint = TraceEnd;

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Owner);
        QueryParams.AddIgnoredActor(this);
        QueryParams.bTraceComplex = true;
        QueryParams.bReturnPhysicalMaterial = true;

        FHitResult HitResult;
        // Blocking hit handler
        if (GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
        {
            AActor* HitActor = HitResult.GetActor();
            EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

            float ActualDamage = BaseDamage;
            if (SurfaceType == SURFACE_FLESHVULNERABLE)
            {
                ActualDamage *= CriticalHitDamageMultiplier;
            }

            UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, HitResult, Owner->GetInstigatorController(), this, DamageType);

            UParticleSystem* SelectedEffect = nullptr;
            switch (SurfaceType)
            {
                // Flesh default & Flesh vulnerable
                case SURFACE_FLESHDEFAULT:
                case SURFACE_FLESHVULNERABLE:
                    SelectedEffect = FleshImpactEffect;
                    break;
                default:
                    SelectedEffect = DefaultImpactEffect;
                    break;
            }

            if (SelectedEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
            }

            TracerEndPoint = HitResult.ImpactPoint;
        }

        if (DebugWeaponDrawing > 0)
        {
            DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
        }

        PlayFireEffects(TracerEndPoint);
        LastFiredTime = GetWorld()->TimeSeconds;
    }
}

void ACSWeapon::StartFire()
{
    float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
    GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ACSWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ACSWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ACSWeapon::BeginPlay()
{
    Super::BeginPlay();
    TimeBetweenShots = 60 / FireRate;
}

void ACSWeapon::PlayFireEffects(FVector TracerEndPoint)
{
    if (MuzzleEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
    }

    if (TracerEffect)
    {
        FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
        UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

        if (TracerComponent)
        {
            TracerComponent->SetVectorParameter(TracerTargetName, TracerEndPoint);
        }
    }

    APawn* Owner = Cast<APawn>(GetOwner());
    if (Owner)
    {
        APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
        if (PlayerController)
        {
            PlayerController->ClientPlayCameraShake(FireCameraShake);
        }
    }

}
