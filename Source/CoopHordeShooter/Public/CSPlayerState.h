#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CSPlayerState.generated.h"


UCLASS()
class COOPHORDESHOOTER_API ACSPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="PlayerState")
    void AddScore(float ScoreDelta);
};
