#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSHealthComponent.generated.h"


// OnHealthChangedEvent
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UCSHealthComponent*, HealthComponent, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COOPHORDESHOOTER_API UCSHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:	
    // Sets default values for this component's properties
    UCSHealthComponent();
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="HealthComponent")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponent")
    float DefaultHealth;

    UFUNCTION()
    void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
                             AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnRep_Health(float OldHealth);
public:
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category="HealthComponent")
    void Heal(float HealAmount);
};
