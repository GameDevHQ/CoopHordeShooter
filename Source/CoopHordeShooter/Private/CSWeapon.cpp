#include "CSWeapon.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "CoopHordeShooter.h"


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
TimeBetweenShots(0.0f),
ScatterOfBulletsMultiplier(100.0f),
ActualScatterOfBulletsMultiplier(0.0f),
SpreadAmount(1000.0f)
{
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    SetReplicates(true);
    NetUpdateFrequency = 80.0f;
    MinNetUpdateFrequency = 40.0f;
}

void ACSWeapon::ServerFire_Implementation()
{
    Fire();
}

bool ACSWeapon::ServerFire_Validate()
{
    return true;
}

void ACSWeapon::OnRep_HitScanTrace()
{
    PlayFireEffects(HitScanTrace.TraceTo);
    PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ACSWeapon::Fire()
{
    if (Role < ROLE_Authority)
    {
        ServerFire();
    }

    // Trace the world from the pawn eyes to crosshair location
    AActor* Owner = GetOwner();
    if (Owner)
    {
        FVector EyeLocation;
        FRotator EyeRotation;
        Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

        FVector ShotDirection = EyeRotation.Vector();
        FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
        
        // Apply some randomness for a shooting weapon
        float RandomX = FMath::FRandRange(-SpreadAmount, SpreadAmount);
        float RandomY = FMath::FRandRange(-SpreadAmount, SpreadAmount);
        float RandomZ = FMath::FRandRange(-SpreadAmount, SpreadAmount);
        FRotator RandomAimRot = EyeRotation + FRotator(RandomX, RandomY, RandomZ);
        TraceEnd += RandomAimRot.Vector() * ActualScatterOfBulletsMultiplier;

        // The particle "Target" parameter
        FVector TracerEndPoint = TraceEnd;

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Owner);
        QueryParams.AddIgnoredActor(this);
        QueryParams.bTraceComplex = true;
        QueryParams.bReturnPhysicalMaterial = true;

        FHitResult HitResult;
        EPhysicalSurface HitSurfaceType = SurfaceType_Default;
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

            UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, HitResult, Owner->GetInstigatorController(), Owner, DamageType);

            PlayImpactEffects(SurfaceType, HitResult.ImpactPoint);
            TracerEndPoint = HitResult.ImpactPoint;
            HitSurfaceType = SurfaceType;
        }

        if (DebugWeaponDrawing > 0)
        {
            DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
        }

        if (ShootWeaponSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, ShootWeaponSound, GetActorLocation());
        }
        
        if (Role == ROLE_Authority)
        {
            HitScanTrace.TraceTo = TracerEndPoint;
            HitScanTrace.SurfaceType = HitSurfaceType;
        }
        
        PlayFireEffects(TracerEndPoint);
        LastFiredTime = GetWorld()->TimeSeconds;
    }
}

void ACSWeapon::StartFire()
{
    float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
    ActualScatterOfBulletsMultiplier = FMath::Lerp(FirstDelay, ScatterOfBulletsMultiplier, 1.f);
    GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ACSWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ACSWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
    ActualScatterOfBulletsMultiplier = 0.0f;
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

void ACSWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
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
        FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
        FVector ShotDirection = ImpactPoint - MuzzleLocation;
        ShotDirection.Normalize();

        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
    }
}

void ACSWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ACSWeapon, HitScanTrace, COND_SkipOwner);
}
