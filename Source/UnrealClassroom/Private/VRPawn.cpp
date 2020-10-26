#include "VRPawn.h"

#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "HammeringStatics.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Components/ActorComponent.h"
#include "VRPhysicalHandActor.h"

AVRPawn::AVRPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    PawnBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PawnBody"));
    SetRootComponent(PawnBody);
    PawnBody->SetCapsuleRadius(34);
    PawnBody->SetCapsuleHalfHeight(88);
    PawnBody->SetSimulatePhysics(false);

    VROffset = CreateDefaultSubobject<USceneComponent>(TEXT("VROffset"));
    VROffset->SetupAttachment(GetRootComponent());

    TeleportationIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportationIndicator"));
    TeleportationIndicator->SetupAttachment(VROffset);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(VROffset);

}

void AVRPawn::BeginPlay()
{
    Super::BeginPlay();

    SpawnHandActors();

    TeleportationIndicator->SetVisibility(false);
}

void AVRPawn::SpawnHandActors()
{
    RightHand = GetWorld()->SpawnActor<AVRPhysicalHandActor>(RightController);
    //RightHand->AttachToComponent(VROffset, FAttachmentTransformRules::KeepRelativeTransform);

    LeftHand = GetWorld()->SpawnActor<AVRPhysicalHandActor>(LeftController);
    //LeftHand->AttachToComponent(VROffset, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVRPawn::GrabAxisRight(float AxisValue)
{
    if (!RightHand->bIsTrackingAxis) { return; }

    RightHand->GrabAxis(AxisValue);
}

void AVRPawn::GrabAxisLeft(float AxisValue)
{
    if (!LeftHand->bIsTrackingAxis) { return; }

    LeftHand->GrabAxis(AxisValue);
}

void AVRPawn::OnGrabRight()
{
    RightAttachedPickup = Cast<IPickupInterface>(RightHand->OnGrab());
    if(RightAttachedPickup!=nullptr)
    {
        if(RightAttachedPickup == LeftAttachedPickup)
        {
            LeftHand->OnRelease();
        }
    }
}

void AVRPawn::OnGrabLeft()
{
    LeftAttachedPickup = Cast<IPickupInterface>(LeftHand->OnGrab());
    if(LeftAttachedPickup!=nullptr)
    {
        if(LeftAttachedPickup == RightAttachedPickup)
        {
            RightHand->OnRelease();
        }
    }
}

void AVRPawn::OnReleaseRight()
{
    RightHand->OnRelease();
}

void AVRPawn::OnReleaseLeft()
{
    LeftHand->OnRelease();
}

void AVRPawn::OnPressTeleportRight()
{
}

void AVRPawn::OnReleaseTeleportRight()
{
}

void AVRPawn::OnPressTeleportLeft()
{
}

void AVRPawn::OnReleaseTeleportLeft()
{
}

void AVRPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &AVRPawn::OnGrabRight);
    PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &AVRPawn::OnGrabLeft);
    PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &AVRPawn::OnReleaseRight);
    PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &AVRPawn::OnReleaseLeft);

    PlayerInputComponent->BindAxis("GrabAxisRight", this, &AVRPawn::GrabAxisRight);
    PlayerInputComponent->BindAxis("GrabAxisLeft", this, &AVRPawn::GrabAxisLeft);

    PlayerInputComponent->BindAction("TeleportRight", IE_Pressed, this, &AVRPawn::OnPressTeleportRight);
    PlayerInputComponent->BindAction("TeleportRight", IE_Released, this, &AVRPawn::OnReleaseTeleportRight);
    PlayerInputComponent->BindAction("TeleportLeft", IE_Pressed, this, &AVRPawn::OnPressTeleportLeft);
    PlayerInputComponent->BindAction("TeleportLeft", IE_Released, this, &AVRPawn::OnReleaseTeleportLeft);
}
