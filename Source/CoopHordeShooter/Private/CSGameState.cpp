#include "CSGameState.h"
#include "Net/UnrealNetwork.h"


void ACSGameState::OnRep_WaveSate(EWaveState OldState)
{
    WaveStateChanged(WaveState, OldState);
}

void ACSGameState::SetWaveState(EWaveState NewState)
{
    if (Role == ROLE_Authority)
    {
        EWaveState OldState = WaveState;
        WaveState = NewState;

        OnRep_WaveSate(OldState);
    }
}

void ACSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSGameState, WaveState);
}
