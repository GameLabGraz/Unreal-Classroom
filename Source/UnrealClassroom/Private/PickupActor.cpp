#include "PickupActor.h"

#include "UnrealClassRoomStatics.h"
#include "Components/SphereComponent.h"

APickupActor::APickupActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
    SetRootComponent(BaseMeshComponent);
    BaseMeshComponent->SetCollisionObjectType(ECC_Grabbable);
    BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMeshComponent->SetSimulatePhysics(true);

    CustomAttachPoint = CreateDefaultSubobject<USphereComponent>(TEXT("CustomAttachPoint"));
    CustomAttachPoint->SetupAttachment(BaseMeshComponent);
    CustomAttachPoint->SetSphereRadius(10.0f);
    CustomAttachPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
    CustomAttachPoint->ShapeColor = FColor::Emerald;
    CustomAttachPoint->SetHiddenInGame(true);

    AttachPositioning = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttackPositioning"));
    AttachPositioning->SetupAttachment(BaseMeshComponent);
    AttachPositioning->SetHiddenInGame(true);
}

void APickupActor::BeginPlay()
{
    Super::BeginPlay();
}

void APickupActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APickupActor::GrabPressed(USceneComponent* AttachTo)
{
    if (BaseMeshComponent == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, FString::Printf(TEXT("BaseMeshComponent is missing")));
        return;
    }
    BaseMeshComponent->SetSimulatePhysics(false);
    BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (bSnapHandToMesh)
    {
        AttachTo->SetWorldLocation(CustomAttachPoint->GetComponentLocation());
        BaseMeshComponent->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepWorldTransform);
    }
    else
    {
        BaseMeshComponent->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepWorldTransform);
    }
}

void APickupActor::GrabReleased()
{
    if (BaseMeshComponent == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, FString::Printf(TEXT("BaseMeshComponent is missing")));
        return;
    }
    BaseMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMeshComponent->SetSimulatePhysics(true);
}

int APickupActor::GetGrabType()
{
    return TypeOfGrab;
}
