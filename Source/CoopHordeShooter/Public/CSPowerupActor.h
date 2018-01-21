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
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Time between powerup ticks
    UPROPERTY(EditDefaultsOnly, Category="Powerups")
    float PowerupInteval;

    // Total times we apply a powerup effect
    UPROPERTY(EditDefaultsOnly, Category="Powerups")
    int TotalTicksCount;

    UFUNCTION()
    void OnTickPowerup();

    int32 TicksProcessed;
    FTimerHandle TimerHandle_PowerupTick;
public:	
    void ActivatePowerup();

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnActivated();

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnPowerupTicked();

    UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
    void OnExpired();
};
