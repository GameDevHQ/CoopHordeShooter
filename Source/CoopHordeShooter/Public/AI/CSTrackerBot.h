#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CSTrackerBot.generated.h"

UCLASS()
class COOPHORDESHOOTER_API ACSTrackerBot : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ACSTrackerBot();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, Category="Components")
    UStaticMeshComponent* MeshComponent;
    
    UPROPERTY(EditDefaultsOnly, Category="AI Movement")
    float MovementForce;

    UPROPERTY(EditDefaultsOnly, Category="AI Movement")
    float DistanceDelta;
    
    UPROPERTY(EditDefaultsOnly, Category="AI Movement")
    bool bUseVelocityChange;

    FVector GetNextPathPoint();
    FVector NextPathPoint;
public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
