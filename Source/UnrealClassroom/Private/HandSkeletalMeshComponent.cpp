#include "HandSkeletalMeshComponent.h"

#include "Components/SphereComponent.h"

UHandSkeletalMeshComponent::UHandSkeletalMeshComponent()
{
    GrabSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereComponent"));
    GrabSphereComponent->SetupAttachment(GetAttachmentRoot());
}
