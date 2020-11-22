// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupActor.h"
#include "LeverInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCLASSROOM_API ALeverInteractableActor : public APickupActor
{
    GENERATED_BODY()

public:
    ALeverInteractableActor();

    UFUNCTION(BlueprintImplementableEvent, Category = "[Property]: Animation")
    void PlayAnimation(bool bIsLeverInActiveMode);

    virtual void GrabReleased() override;
    virtual void GrabPressed(USceneComponent* AttachTo) override;

    // Members
public:

    UPROPERTY(EditAnywhere, Category = "[Property]: Components")
    UStaticMeshComponent* LeverMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[Property]: Function")
    bool IsLeverInActiveMode = false;

private:
    UPROPERTY(Transient)
    USceneComponent* AttachedHand;
};
