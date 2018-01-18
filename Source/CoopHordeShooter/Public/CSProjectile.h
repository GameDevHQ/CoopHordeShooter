#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSProjectile.generated.h"


class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class COOPHORDESHOOTER_API ACSProjectile : public AActor
{
    GENERATED_BODY()
	
public:	
    ACSProjectile();

protected:
    virtual void BeginPlay() override;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
    float Damage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
    float DamageRadius;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FX")
    UParticleSystem* ExplosionEffect;
private:
    FTimerHandle EffectTimerHandler;

    UFUNCTION()
    void PlayExplosionEffect();
};
