#pragma once

#include "CoreMinimal.h"

#include "Components/SkeletalMeshComponent.h"
#include "HandSkeletalMeshComponent.generated.h"

class USphereComponent;

UENUM()
enum EHandType
{
    RightHand UMETA(DisplayName = "Right"),
    LeftHand UMETA(DisplayName = "Left"),
};

UCLASS()
class UNREALCLASSROOM_API UHandSkeletalMeshComponent : public USkeletalMeshComponent
{
    GENERATED_BODY()

public:
    UHandSkeletalMeshComponent();

public:

    UPROPERTY(EditAnywhere, Category = "[Property]: Components")
    USphereComponent* GrabSphereComponent;

    UPROPERTY(EditAnywhere, Category = "[Property]: Hand")
    TEnumAsByte<EHandType> HandType = RightHand;
    
    
};
