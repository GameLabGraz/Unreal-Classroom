#pragma once

#include "CoreMinimal.h"

#include "PickupInterface.h"
#include "GameFramework/Actor.h"

#include "PickupActor.generated.h"

class USphereComponent;

UCLASS()
class UNREALCLASSROOM_API APickupActor : public AActor, public IPickupInterface
{
    GENERATED_BODY()

public:
    APickupActor();

    virtual void GrabPressed(USceneComponent* AttachTo) override;
    virtual void GrabReleased() override;
    virtual int GetGrabType() override;

protected:
    virtual void BeginPlay() override;

    FVector GetCustomAttachLocation() const;
    FRotator GetCustomAttachRotation() const;

public:
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "[Properties]: Attach Behaviour")
    USphereComponent* CustomAttachPoint;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bSnapToHand = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bUseCustomRotation = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bUseCustomLocation = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bIsShowingDebug = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Animation")
    TEnumAsByte<EGrabType> TypeOfGrab;
};
