#include "CSPowerupActor.h"


// Sets default values
ACSPowerupActor::ACSPowerupActor():
PowerupInteval(0.0f),
TotalTicksCount(0),
TicksProcessed(0)
{
}

// Called when the game starts or when spawned
void ACSPowerupActor::BeginPlay()
{
    Super::BeginPlay();
}

void ACSPowerupActor::OnTickPowerup()
{
    TicksProcessed++;
    OnPowerupTicked();

    if (TicksProcessed >= TotalTicksCount)
    {
        OnExpired();
        GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
    }
}

void ACSPowerupActor::ActivatePowerup()
{
    if (PowerupInteval > 0.0f)
    {
        GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ACSPowerupActor::OnTickPowerup, PowerupInteval, true, 0.0f);
    }
    else
    {
        OnTickPowerup();
    }
}
