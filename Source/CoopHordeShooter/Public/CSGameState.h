#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CSGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState: uint8
{
    WaitingToStart,
    WaveInProgress,
    WaitingToComplete,
    WaveComplete,
    GameOver
};

UCLASS()
class COOPHORDESHOOTER_API ACSGameState : public AGameStateBase
{
    GENERATED_BODY()
protected:
    UFUNCTION()
    void OnRep_WaveSate(EWaveState OldState);
    
    UFUNCTION(BlueprintImplementableEvent, Category="GameState")
    void WaveStateChanged(EWaveState NewState, EWaveState OldState);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveSate, Category="GameState")
    EWaveState WaveState;
public:
    void SetWaveState(EWaveState NewState);
};
