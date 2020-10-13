#pragma once

#include "CoreMinimal.h"
#include "PickupActor.h"

#include "PhysicalConstraintPickupActor.generated.h"

class UPhysicsConstraintComponent;


UCLASS()
class UNREALCLASSROOM_API APhysicalConstraintPickupActor : public APickupActor
{
    GENERATED_BODY()

public:

    APhysicalConstraintPickupActor();

protected:

    UPROPERTY(VisibleAnywhere)
    UPhysicsConstraintComponent* PhysicsConstraintComponent;

    virtual void GrabPressed(USceneComponent* AttachTo) override;
    virtual void GrabReleased() override;
};
