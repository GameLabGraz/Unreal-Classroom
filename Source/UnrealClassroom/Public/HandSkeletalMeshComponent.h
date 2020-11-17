#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "HandSkeletalMeshComponent.generated.h"

class USphereComponent;

UCLASS()
class UNREALCLASSROOM_API UHandSkeletalMeshComponent : public USkeletalMeshComponent
{
    GENERATED_BODY()

public:
    UHandSkeletalMeshComponent();

public:

    UPROPERTY(EditAnywhere, Category = "[Property]: Components")
    USphereComponent* GrabSphereComponent;
};
