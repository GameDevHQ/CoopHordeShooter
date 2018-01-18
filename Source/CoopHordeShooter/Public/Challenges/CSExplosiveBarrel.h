#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSExplosiveBarrel.generated.h"


class UStaticMeshComponent;
class URadialForceComponent;
class UCSHealthComponent;
class UParticleSystem;
class UMaterialInterface;


UCLASS()
class COOPHORDESHOOTER_API ACSExplosiveBarrel : public AActor
{
    GENERATED_BODY()

public:	
    // Sets default values for this actor's properties
    ACSExplosiveBarrel();
protected:
    UPROPERTY(ReplicatedUsing=OnRep_Exploded)
    bool bIsExploded;

    UFUNCTION()
    void OnRep_Exploded();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UCSHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    URadialForceComponent* RadialForceComponent;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    float ExplosionForce;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UMaterialInterface* ExplodedMaterial;

    UFUNCTION()
    void OnHealthChanged(UCSHealthComponent* HealthComp, float Health,
                        float HealthDelta, const class UDamageType* DamageType,
                        class AController* InstigatedBy, AActor* DamageCauser);

    void PlayExplosionEffects();
};
