#include "CSGameMode.h"
#include "TimerManager.h"

ACSGameMode::ACSGameMode():
BotsToSpawn(0),
WaveCount(0),
TimeBetweenWaves(10.0f)
{ 
}

void ACSGameMode::StartWave()
{
    WaveCount++;
    BotsToSpawn = 2 * WaveCount;
    GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ACSGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
}

void ACSGameMode::EndWave()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
}

void ACSGameMode::PrepareForNextWave()
{
    GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ACSGameMode::StartWave, TimeBetweenWaves, false);
}

void ACSGameMode::SpawnBotTimerElapsed()
{
    SpawnNewBot();

    BotsToSpawn--;
    if (BotsToSpawn <= 0)
    {
        EndWave();
    }
}

void ACSGameMode::StartPlay()
{
    Super::StartPlay();
    PrepareForNextWave();
}
