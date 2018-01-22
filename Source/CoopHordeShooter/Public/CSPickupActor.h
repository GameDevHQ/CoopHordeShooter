#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSPickupActor.generated.h"


class USphereComponent;
class UDecalComponent;
class ACSPowerupActor;

UCLASS()
class COOPHORDESHOOTER_API ACSPickupActor : public AActor
{
    GENERATED_BODY()

public:	
    // Sets default values for this actor's properties
    ACSPickupActor();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category="Components")
    USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, Category="Components")
    UDecalComponent* DecalComponent;

    UPROPERTY(EditDefaultsOnly, Category="PickupActor")
    TSubclassOf<ACSPowerupActor> PowerUpClass;

    UPROPERTY(EditDefaultsOnly, Category="Gameplay")
    float RespawnTime;

    void Respawn();

    ACSPowerupActor* PowerUpInstance;
    FTimerHandle TimerHandle_RespawnPowerUp;
public:
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

