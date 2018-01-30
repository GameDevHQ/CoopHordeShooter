#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CSTrackerBot.generated.h"


class UCSHealthComponent;
class UMaterialInstanceDynamic;
class USphereComponent;
class USoundCue;

UCLASS()
class COOPHORDESHOOTER_API ACSTrackerBot : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ACSTrackerBot();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, Category="Components")
    UStaticMeshComponent* MeshComponent;
    
    UPROPERTY(VisibleDefaultsOnly, Category="Components")
    UCSHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, Category="Components")
    USphereComponent* SphereComponent;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, Category="AI Movement")
    float MovementForce;

    UPROPERTY(EditDefaultsOnly, Category="AI Movement")
    float DistanceDelta;
    
    UPROPERTY(EditDefaultsOnly, Category="AI Movement")
    bool bUseVelocityChange;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay")
    float ExplosionDamage;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay")
    float ExplosionRadius;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay")
    float SelfDamageInterval;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay", meta=(ClampMin=0))
    int MaxPowerLevel;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay", meta=(ClampMin=0))
    float NearbyAreaRadius;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay", meta=(ClampMin=0))
    float CheckNearbyBotsInterval;

    UPROPERTY(EditDefaultsOnly, Category="Sound effects")
    USoundCue* SelfDestructSound;

    UPROPERTY(EditDefaultsOnly, Category="Sound effects")
    USoundCue* ExplodeSound;

    UFUNCTION()
    void HandleTakeDamage(UCSHealthComponent* HealthComp, float Health,
                          float HealthDelta, const class UDamageType* DamageType, 
                          class AController* InstigatedBy, AActor* DamageCauser);

    void SelfDestruct();
    void DamageSelf();
    void CheckNearbyBots();
    FVector GetNextPathPoint();
    void RefreshPath();

    int PowerLevel;
    bool bExploded;
    bool bStartedSelfDestruction;

    FVector NextPathPoint;
    UMaterialInstanceDynamic* MaterialInstance;
    FTimerHandle TimerHandle_SelfDamage;
    FTimerHandle TimerHandle_CheckNearbyBots;
    FTimerHandle TimerHandle_UpdatePath;
public:	
    virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
