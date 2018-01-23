#include "CSPowerupActor.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ACSPowerupActor::ACSPowerupActor():
PowerupInteval(0.0f),
TotalTicksCount(0),
TicksProcessed(0),
bIsActivated(false)
{
    SetReplicates(true);
}

void ACSPowerupActor::OnTickPowerup()
{
    TicksProcessed++;
    OnPowerupTicked();

    if (TicksProcessed >= TotalTicksCount)
    {
        OnExpired();

        bIsActivated = false;
        OnRep_PowerupActive();

        GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
    }
}

void ACSPowerupActor::OnRep_PowerupActive()
{
   OnPowerupStateChanged(bIsActivated);
}

void ACSPowerupActor::ActivatePowerup()
{
    OnActivated();

    bIsActivated = true;
    OnRep_PowerupActive();

    if (PowerupInteval > 0.0f)
    {
        GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ACSPowerupActor::OnTickPowerup, PowerupInteval, true);
    }
    else
    {
        OnTickPowerup();
    }
}

void ACSPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACSPowerupActor, bIsActivated);
}
