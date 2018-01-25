#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CSGameMode.generated.h"

enum class EWaveState: uint8;

UCLASS()
class COOPHORDESHOOTER_API ACSGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ACSGameMode();
protected:
    
    UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
    void SpawnNewBot();

    UPROPERTY(EditDefaultsOnly, Category="GameMode", meta=(ClampMin=0))
    float TimeBetweenWaves;

    void StartWave();
    void EndWave();
    void CheckWaveState();
    void PrepareForNextWave();
    void SpawnBotTimerElapsed();
    
    void CheckAnyPlayerIsAlive();
    void GameOver();
    void SetWaveState(EWaveState NewState);

    int BotsToSpawn;
    int WaveCount; 
    FTimerHandle TimerHandle_BotSpawner;
    FTimerHandle TimerHandle_NextWaveStart;
public:
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;
};
