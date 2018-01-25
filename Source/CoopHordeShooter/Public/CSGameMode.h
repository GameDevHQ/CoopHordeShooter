#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CSGameMode.generated.h"


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
    void PrepareForNextWave();
    void SpawnBotTimerElapsed();

    int BotsToSpawn;
    int WaveCount; 
    FTimerHandle TimerHandle_BotSpawner;
    FTimerHandle TimerHandle_NextWaveStart;
public:
    virtual void StartPlay() override;
};
