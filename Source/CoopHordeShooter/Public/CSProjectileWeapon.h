#pragma once

#include "CoreMinimal.h"
#include "CSWeapon.h"
#include "CSProjectileWeapon.generated.h"


UCLASS()
class COOPHORDESHOOTER_API ACSProjectileWeapon : public ACSWeapon
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile weapon")
    TSubclassOf<AActor> ProjectileClass;

    virtual void Fire() override;
};
