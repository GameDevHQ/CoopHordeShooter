// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComponent;
public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
