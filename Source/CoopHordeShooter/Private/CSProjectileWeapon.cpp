#include "CSProjectileWeapon.h"
#include "Components/SkeletalMeshComponent.h"


void ACSProjectileWeapon::Fire()
{
    // Trace the world from the pawn eyes to crosshair location
    AActor* Owner = GetOwner();
    if (Owner && ProjectileClass)
    {
        FVector EyeLocation;
        FRotator EyeRotation;
        Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
    
        FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParameters);
    }
}
