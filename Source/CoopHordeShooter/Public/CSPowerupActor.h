#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSPowerupActor.generated.h"


UCLASS()
class COOPHORDESHOOTER_API ACSPowerupActor : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    ACSPowerupActor();
protected:
    // Time between powerup ticks
    UPROPERTY(EditDefaultsOnly, Category="Powerups")
    float PowerupInteval;

    // Total times we apply a powerup effect
    UPROPERTY(EditDefaultsOnly, Category="Powerups")
    int TotalTicksCount;

    UFUNCTION()
    void OnTickPowerup();

    UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
    bool bIsActivated;

    UFUNCTION()
    void OnRep_PowerupActive();

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnPowerupStateChanged(bool bNewIsActive);

    int32 TicksProcessed;
    FTimerHandle TimerHandle_PowerupTick;
public:	
    void ActivatePowerup(AActor* ActiveFor);

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnActivated(AActor* ActiveFor);

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnPowerupTicked();

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnExpired();
};
