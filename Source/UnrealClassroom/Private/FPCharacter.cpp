#include "FPCharacter.h"


#include "GrabbableInterface.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "HandSkeletalMeshComponent.h"
#include "LeverActor.h"
#include "PickupActor.h"
#include "Components/ArrowComponent.h"

AFPCharacter::AFPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set Default Capsule collider size
    GetCapsuleComponent()->InitCapsuleSize(20.f, 96.0f);

    FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
    FPCamera->SetupAttachment(GetCapsuleComponent());
    FPCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position of Camera based of FPS camera
    FPCamera->bUsePawnControlRotation = true;

    RightHandComponent = CreateDefaultSubobject<UHandSkeletalMeshComponent>(TEXT("RightHand"));
    RightHandComponent->SetupAttachment(FPCamera);
    RightHandComponent->HandType = RightHand;
    RightHandComponent->SetRelativeLocation(FVector(60.0f, 20.0f, -12.0f));
    RightHandComponent->GrabSphereComponent->SetRelativeLocation(FVector(6, 2, 0));

    LeftHandComponent = CreateDefaultSubobject<UHandSkeletalMeshComponent>(TEXT("LeftHand"));
    LeftHandComponent->SetupAttachment(FPCamera);
    LeftHandComponent->HandType = LeftHand;
    LeftHandComponent->SetRelativeLocation(FVector(60.0f, -20.0f, -12.0f));
    LeftHandComponent->GrabSphereComponent->SetRelativeLocation(FVector(6, -2, 0));
}

void AFPCharacter::BeginPlay()
{
    Super::BeginPlay();

    InitialRelativeLocationRightHand = RightHandComponent->GetRelativeLocation();
    InitialRelativeLocationLeftHand = LeftHandComponent->GetRelativeLocation();
    InitialRelativeRotationRightHand = RightHandComponent->GetRelativeRotation();
    InitialRelativeRotationLeftHand = LeftHandComponent->GetRelativeRotation();
}

void AFPCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    GrabRaycast();
}

void AFPCharacter::GrabRaycast()
{
    auto const StartLocation = FPCamera->GetComponentLocation();
    auto const EndLocation = StartLocation + FPCamera->GetForwardVector() * (MaxGrabDistance + HandDimension);
    IsGrabRaycastHit = GetWorld()->LineTraceSingleByChannel(GrabRaycastResult, StartLocation, EndLocation,
                                                            ECC_Visibility);
    if (IsGrabRaycastHit)
    {
        const auto bImplementedGrabbable = GrabRaycastResult.Actor->GetClass()->ImplementsInterface(
            UGrabbableInterface::StaticClass());
        if (bImplementedGrabbable)
        {
            SuspectActorForGrab = GrabRaycastResult.GetActor();
        }
    }
    else
    {
        SuspectActorForGrab = nullptr;
    }
}


void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("InteractionRight", EInputEvent::IE_Pressed, this,
                                     &AFPCharacter::InteractionRightPressed);
    PlayerInputComponent->BindAction("InteractionRight", EInputEvent::IE_Released, this,
                                     &AFPCharacter::InteractionRightReleased);
    PlayerInputComponent->BindAction("InteractionLeft", EInputEvent::IE_Pressed, this,
                                     &AFPCharacter::InteractionLeftPressed);
    PlayerInputComponent->BindAction("InteractionLeft", EInputEvent::IE_Released, this,
                                     &AFPCharacter::InteractionLeftReleased);

    PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);

    PlayerInputComponent->BindAxis("TurnAtRate", this, &AFPCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookupAtRate", this, &AFPCharacter::LookupAtRate);

    PlayerInputComponent->BindAxis("GrabRight", this, &AFPCharacter::GrabRight);
    PlayerInputComponent->BindAxis("GrabLeft", this, &AFPCharacter::GrabLeft);
}

void AFPCharacter::InteractionRightPressed()
{
    if (RightGrabbedActor != nullptr)
    {
        Cast<IGrabbableInterface>(RightGrabbedActor)->GrabReleased();
        RightGrabbedActor = nullptr;
        SuspectActorForGrab = nullptr;
        return;
    }

    if (bIsReadyForGrabRight)
    {
        const auto GrabTarget = Cast<IGrabbableInterface>(SuspectActorForGrab);
        if (GrabTarget != nullptr)
        {
            GrabTarget->GrabPressed(RightHandComponent);
            RightGrabbedActor = SuspectActorForGrab;
        }
    }
}

void AFPCharacter::InteractionRightReleased()
{
    // if (RightGrabbedActor == nullptr) { return; }
    // Cast<IGrabbableInterface>(RightGrabbedActor)->GrabReleased();
    // RightGrabbedActor = nullptr;
}

void AFPCharacter::InteractionLeftPressed()
{
    if (LeftGrabbedActor != nullptr)
    {
        Cast<IGrabbableInterface>(LeftGrabbedActor)->GrabReleased();
        LeftGrabbedActor = nullptr;
        SuspectActorForGrab = nullptr;
        return;
    }

    if (bIsReadyForGrabLeft)
    {
        const auto GrabTarget = Cast<IGrabbableInterface>(SuspectActorForGrab);
        if (GrabTarget != nullptr)
        {
            GrabTarget->GrabPressed(LeftHandComponent);
            LeftGrabbedActor = SuspectActorForGrab;
        }
    }
}

void AFPCharacter::InteractionLeftReleased()
{
}

AActor* AFPCharacter::FindNearestGrabbableActor(USphereComponent* HandGrabSphere)
{
    AActor* GrabTarget = nullptr;

    TArray<UPrimitiveComponent*> OverlappingComponents;
    HandGrabSphere->GetOverlappingComponents(OverlappingComponents);

    auto ShortestDistance = TNumericLimits<float>::Max();
    for (auto Comp : OverlappingComponents)
    {
        if (Comp->GetOwner()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()))
        {
            const auto Distance = FVector::Distance(Comp->GetComponentLocation(),
                                                    HandGrabSphere->GetComponentLocation());
            if (Distance < ShortestDistance)
            {
                ShortestDistance = Distance;
                GrabTarget = Comp->GetOwner();
            }
        }
    }

    return GrabTarget;
}

void AFPCharacter::ResetHand(EHandType HandType)
{
    if (HandType == RightHand)
    {
        RightHandComponent->SetRelativeLocation(InitialRelativeLocationRightHand);
        RightHandComponent->SetRelativeRotation(InitialRelativeRotationRightHand);
    }
    else if (HandType == LeftHand)
    {
        LeftHandComponent->SetRelativeLocation(InitialRelativeLocationLeftHand);
        LeftHandComponent->SetRelativeRotation(InitialRelativeRotationLeftHand);
    }
}

void AFPCharacter::ReAttachHand(EHandType HandType)
{
    if (HandType == RightHand)
    {
        RightHandComponent->AttachToComponent(FPCamera, FAttachmentTransformRules::KeepWorldTransform);
        ResetHand(RightHand);
    }
    else if (HandType == LeftHand)
    {
        LeftHandComponent->AttachToComponent(FPCamera, FAttachmentTransformRules::KeepWorldTransform);
        ResetHand(LeftHand);
    }
}


void AFPCharacter::MoveForward(float Value)
{
    if (IsDebuggingController)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Move Forward: %f "), Value));
    }

    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void AFPCharacter::MoveRight(float Value)
{
    if (IsDebuggingController)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Move Right: %f "), Value));
    }

    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void AFPCharacter::TurnAtRate(float Value)
{
    AddControllerYawInput(Value * TurnBaseRate * GetWorld()->GetDeltaSeconds());
}

void AFPCharacter::LookupAtRate(float Value)
{
    AddControllerPitchInput(Value * LookupBaseRate * GetWorld()->GetDeltaSeconds());
}

void AFPCharacter::GrabRight(float Value)
{
    Value = FMath::Clamp(Value, 0.0f, 1.0f);
    auto MinLocation = RightHandComponent->GetComponentLocation();

    if (Value < 0.01)
    {
        ResetHand(RightHand);
        MinLocation = RightHandComponent->GetComponentLocation();

        if (RightGrabbedActor != nullptr)
        {
            const auto GrabTarget = Cast<IGrabbableInterface>(RightGrabbedActor);
            if (GrabTarget != nullptr)
            {
                GrabTarget->GrabReleased();
                RightGrabbedActor = nullptr;
                SuspectActorForGrab = nullptr;
            }
        }
    }

    if (SuspectActorForGrab != nullptr)
    {
        const auto GrabTarget = Cast<APickupActor>(SuspectActorForGrab);

        if (GrabTarget != nullptr)
        {
            const auto MaxLocation = GrabTarget->CustomAttachPoint->GetComponentLocation();
            const auto NewWorldLocation = FMath::Lerp(MinLocation, MaxLocation, Value);
            RightHandComponent->SetWorldLocation(NewWorldLocation);

            if (Value > 0.95f)
            {
                bIsReadyForGrabRight = true;
                return;
            }
        }
    }
    else
    {
        RightHandComponent->SetRelativeLocation(InitialRelativeLocationRightHand);

        auto GrabRange = MaxGrabDistance;
        if (IsGrabRaycastHit)
        {
            GrabRange = FVector::Distance(FPCamera->GetComponentLocation(), GrabRaycastResult.Location) - HandDimension;
        }

        const auto NewWorldLocation = FMath::Lerp(MinLocation,
                                                  FPCamera->GetComponentLocation() + FPCamera->GetForwardVector().
                                                  GetSafeNormal() * GrabRange, Value);
        RightHandComponent->SetWorldLocation(NewWorldLocation);
    }

    bIsReadyForGrabRight = false;
}

void AFPCharacter::GrabLeft(float Value)
{
    Value = FMath::Clamp(Value, 0.0f, 1.0f);
    auto MinLocation = LeftHandComponent->GetComponentLocation();

    if (Value < 0.01)
    {
        ResetHand(LeftHand);
        MinLocation = LeftHandComponent->GetComponentLocation();

        if (LeftGrabbedActor != nullptr)
        {
            const auto GrabTarget = Cast<IGrabbableInterface>(LeftGrabbedActor);
            if (GrabTarget != nullptr)
            {
                GrabTarget->GrabReleased();
                LeftGrabbedActor = nullptr;
                SuspectActorForGrab = nullptr;
            }
        }
    }

    if (SuspectActorForGrab != nullptr)
    {
        const auto GrabTarget = Cast<APickupActor>(SuspectActorForGrab);

        if (GrabTarget != nullptr)
        {
            const auto MaxLocation = GrabTarget->CustomAttachPoint->GetComponentLocation();
            const auto NewWorldLocation = FMath::Lerp(MinLocation, MaxLocation, Value);
            LeftHandComponent->SetWorldLocation(NewWorldLocation);

            if (Value > 0.95f)
            {
                bIsReadyForGrabLeft = true;
                return;
            }
        }
    }
    else
    {
        auto GrabRange = MaxGrabDistance;
        if (IsGrabRaycastHit)
        {
            GrabRange = FVector::Distance(FPCamera->GetComponentLocation(), GrabRaycastResult.Location) - HandDimension;
        }

        const auto NewWorldLocation = FMath::Lerp(MinLocation,
                                                  FPCamera->GetComponentLocation() + FPCamera->GetForwardVector().
                                                  GetSafeNormal() * GrabRange, Value);
        LeftHandComponent->SetWorldLocation(NewWorldLocation);
    }

    bIsReadyForGrabLeft = false;
}
