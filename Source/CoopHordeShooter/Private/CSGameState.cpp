#include "CSGameState.h"
#include "Net/UnrealNetwork.h"


void ACSGameState::OnRep_WaveSate(EWaveState OldState)
{
    WaveStateChanged(WaveState, OldState);
}

void ACSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSGameState, WaveState);
}
