#include "CSGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "CSHealthComponent.h"


ACSGameMode::ACSGameMode():
BotsToSpawn(0),
WaveCount(0),
TimeBetweenWaves(10.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.0f;
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

void ACSGameMode::CheckWaveState()
{
    bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
    if (BotsToSpawn > 0 || bIsPreparingForWave)
    {
        return;
    }

    bool bWaveIsPassed = true;
    for (FConstPawnIterator It=GetWorld()->GetPawnIterator(); It ; ++It)
    {
        APawn* TestPawn = It->Get();
        if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
        {
            continue;
        }

        UCSHealthComponent* HealthComponent = Cast<UCSHealthComponent>(TestPawn->GetComponentByClass(UCSHealthComponent::StaticClass()));
        if (HealthComponent && HealthComponent->GetHealth() >= 0.0f)
        {
            bWaveIsPassed = false;
            break;
        }
    }

    if (bWaveIsPassed)
    {
        PrepareForNextWave();
    }
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

void ACSGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    CheckWaveState();
}
