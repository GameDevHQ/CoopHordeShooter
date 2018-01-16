#include "CSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CSWeapon.h"
#include "Components/CSHealthComponent.h"
#include "CoopHordeShooter.h"


// Sets default values
ACSCharacter::ACSCharacter():
DefaultFOV(90.0f),
ZoomedFOV(65.0f),
ZoomInterpolateSpeed(20.0f),
WeaponAttackSocketName("WeaponSocket")
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetupAttachment(RootComponent);

    // Set the default camera for a character
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UCSHealthComponent>(TEXT("HealthComponent"));

    GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

    GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
    GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
}

void ACSCharacter::MoveForward(float Value)
{
    AddMovementInput(GetActorForwardVector() * Value);
}

void ACSCharacter::MoveRight(float Value)
{
    AddMovementInput(GetActorRightVector() * Value);
}

// Called when the game starts or when spawned
void ACSCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    DefaultFOV = CameraComponent->FieldOfView;
    HealthComponent->OnHealthChanged.AddDynamic(this, &ACSCharacter::OnHealthChanged);

    if (Role  == ROLE_Authority)
    {
        // Spawn a default weapon
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        CurrentWeapon = GetWorld()->SpawnActor<ACSWeapon>(DefaultWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);

        if (CurrentWeapon)
        {
            CurrentWeapon->SetOwner(this);
            CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttackSocketName);
        }
    }
}

void ACSCharacter::BeginCrouch()
{
    Crouch();
}

void ACSCharacter::EndCrouch()
{
    UnCrouch();
}

void ACSCharacter::BeginZoom()
{
    bWantsToZoom = true;
}

void ACSCharacter::EndZoom()
{
    bWantsToZoom = false;
}

void ACSCharacter::StartFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StartFire();
    }
}

void ACSCharacter::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}


void ACSCharacter::OnHealthChanged(UCSHealthComponent* HealthComp, float Health,
                                   float HealthDelta, const class UDamageType* DamageType,
                                   class AController* InstigatedBy, AActor* DamageCauser)
{
    // A character should play dia animation when no health
    if (Health <= 0.0f && !bDied)
    {
        bDied = true;

        GetMovementComponent()->StopMovementImmediately();
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        DetachFromControllerPendingDestroy();
        SetLifeSpan(10.0f);
    }

}

// Called every frame
void ACSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
    float NewFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpolateSpeed);
    CameraComponent->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind WSAD keys 
    PlayerInputComponent->BindAxis("MoveForward", this, &ACSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACSCharacter::MoveRight);

    // Bind mouse for look up aroud
    PlayerInputComponent->BindAxis("LookUp", this, &ACSCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Turn", this, &ACSCharacter::AddControllerYawInput);

    // Crouch
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACSCharacter::BeginCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACSCharacter::EndCrouch);

    // Jump
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

    // Zoom In
    PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACSCharacter::BeginZoom);
    PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACSCharacter::EndZoom);

    // Fire
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACSCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACSCharacter::StopFire);
}

FVector ACSCharacter::GetPawnViewLocation() const
{
    if (CameraComponent)
    {
        return CameraComponent->GetComponentLocation();
    }

    return Super::GetPawnViewLocation();
}

void ACSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSCharacter, CurrentWeapon);
}
