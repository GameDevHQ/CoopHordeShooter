#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "CSCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPHORDESHOOTER_API ACSCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    // Sets default values for this character's properties
    ACSCharacter();

    // Keyboard moving WSAD
    void MoveForward(float Value);
    void MoveRight(float Value);

    float DefaultFOV;
    bool bWantsToZoom;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void BeginCrouch();
    void EndCrouch();

    void BeginZoom();
    void EndZoom();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Player")
    float ZoomedFOV;

    UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
    float ZoomInterpolateSpeed;
public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual FVector GetPawnViewLocation() const override;
};
