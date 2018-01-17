#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSWeapon.generated.h"


class USkeletalMeshComponent;
class UParticleSystem;
class UCameraShake;
class USoundBase;


// Contains an information of a single hitscan weapon
USTRUCT()
struct FHitScanTrace
{
    GENERATED_BODY()
public:
    UPROPERTY()
    FVector_NetQuantize TraceFrom;
    
    UPROPERTY()
    FVector_NetQuantize TraceTo;
};

UCLASS()
class COOPHORDESHOOTER_API ACSWeapon : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    ACSWeapon();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    void PlayFireEffects(FVector TracerEndPoint);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName TracerTargetName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    USoundBase* ShootWeaponSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* MuzzleEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* DefaultImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* FleshImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* TracerEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<UCameraShake> FireCameraShake;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float BaseDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float CriticalHitDamageMultiplier;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 1, ClampMax = 1000))
    float FireRate;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 1, ClampMax = 1000))
    float ScatterOfBulletsMultiplier;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0))
    float SpreadAmount;

    FTimerHandle TimerHandle_TimeBetweenShots;
    float LastFiredTime;
    float TimeBetweenShots;
    float ActualScatterOfBulletsMultiplier;

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFire();

    UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
    FHitScanTrace HitScanTrace;

    UFUNCTION()
    void OnRep_HitScanTrace();
public:
    virtual void Fire();
    void StartFire();
    void StopFire();
};
