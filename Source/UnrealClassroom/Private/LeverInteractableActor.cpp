#include "LeverInteractableActor.h"


#include "FPCharacter.h"
#include "HandSkeletalMeshComponent.h"
#include "UnrealClassRoomStatics.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ALeverInteractableActor::ALeverInteractableActor(): Super()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(BaseMeshComponent);
    BaseMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
    BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BaseMeshComponent->SetSimulatePhysics(false);

    LeverMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("LeverMeshComponent");
    LeverMeshComponent->SetupAttachment(BaseMeshComponent);
    LeverMeshComponent->SetCollisionObjectType(ECC_Grabbable);
    LeverMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    LeverMeshComponent->SetSimulatePhysics(false);

    //CustomAttachPoint = CreateDefaultSubobject<USphereComponent>(TEXT("CustomAttachPoint"));
    CustomAttachPoint->SetupAttachment(LeverMeshComponent);
    CustomAttachPoint->SetSphereRadius(10.0f);
    CustomAttachPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
    CustomAttachPoint->ShapeColor = FColor::Emerald;
    CustomAttachPoint->SetHiddenInGame(false);
}

void ALeverInteractableActor::GrabReleased()
{
    Super::GrabReleased();

    auto PC = Cast<AFPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (PC != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue,
                                         FString::Printf(
                                             TEXT("Player Controller is found")));

        if(AttachedHand == nullptr){return;}
        AttachedHand->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        const auto Hand = Cast<UHandSkeletalMeshComponent>(AttachedHand);
        if (Hand != nullptr)
        {
            PC->ReAttachHand(Hand->HandType);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue,
                                         FString::Printf(
                                             TEXT("There is no AFPCharacter: %s"),
                                             *UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetName()));
    }
}

void ALeverInteractableActor::GrabPressed(USceneComponent* AttachTo)
{
    Super::GrabPressed(AttachTo);

    AttachedHand = AttachTo;
    AttachTo->AttachToComponent(CustomAttachPoint, FAttachmentTransformRules::KeepWorldTransform);

    PlayAnimation(IsLeverInActiveMode);
    IsLeverInActiveMode = !IsLeverInActiveMode;
}
