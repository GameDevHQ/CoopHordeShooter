#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "CSCharacter.generated.h"


class ACSWeapon;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPHORDESHOOTER_API ACSCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    // Sets default values for this character's properties
    ACSCharacter();
protected:
    float DefaultFOV;
    bool bWantsToZoom;

    ACSWeapon* CurrentWeapon;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Keyboard moving WSAD
    void MoveForward(float Value);
    void MoveRight(float Value);

    void BeginCrouch();
    void EndCrouch();

    void BeginZoom();
    void EndZoom();

    void StartFire();
    void StopFire();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Player")
    float ZoomedFOV;

    UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
    float ZoomInterpolateSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "Player")
    TSubclassOf<ACSWeapon> DefaultWeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Player")
    FName WeaponAttackSocketName;
public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual FVector GetPawnViewLocation() const override;
};
