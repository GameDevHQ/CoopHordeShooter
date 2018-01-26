#include "CSHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "CSGameMode.h"


// Sets default values for this component's properties
UCSHealthComponent::UCSHealthComponent():
DefaultHealth(100.f),
bIsDead(false)
{
    SetIsReplicated(true);
}

// Called when the game starts
void UCSHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwnerRole() == ROLE_Authority)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            Owner->OnTakeAnyDamage.AddDynamic(this, &UCSHealthComponent::HandleTakeAnyDamage);
        }
    }

    Health = DefaultHealth;
}

void UCSHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
                                             AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
    OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

    UE_LOG(LogTemp, Log, TEXT("Health changed: %s"), *FString::SanitizeFloat(Health));

    bIsDead = Health <= 0.0f;
    if (bIsDead)
    {
        ACSGameMode* GameMode = Cast<ACSGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);   
        }
    }
}

void UCSHealthComponent::OnRep_Health(float OldHealth)
{
    float Damage = Health - OldHealth;
    OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void UCSHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.0f || Health <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
    OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);

    UE_LOG(LogTemp, Log, TEXT("Health changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));
}

float UCSHealthComponent::GetHealth() const
{
    return Health;
}

void UCSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCSHealthComponent, Health);
}
