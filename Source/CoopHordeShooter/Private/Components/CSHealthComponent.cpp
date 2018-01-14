#include "CSHealthComponent.h"


// Sets default values for this component's properties
UCSHealthComponent::UCSHealthComponent():
DefaultHealth(100.f)
{
}


// Called when the game starts
void UCSHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UCSHealthComponent::HandleTakeAnyDamage);
    }

    Health = DefaultHealth;
}

void UCSHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, 
                                            const UDamageType* DamageType, AController* InstigatedBy, 
                                            AActor* DamageCauser)
{
    if (Damage <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
    OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}
