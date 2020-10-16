#include "HammeringPawn.h"

#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineMeshComponent.h"
#include "HammeringStatics.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"


AHammeringPawn::AHammeringPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    // SetRootComponent(RootSceneComponent);

    PawnBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PawnBody"));
    SetRootComponent(PawnBody);
    // PawnBody->SetupAttachment(GetRootComponent());
    PawnBody->SetCapsuleRadius(34);
    PawnBody->SetCapsuleHalfHeight(88);
    PawnBody->SetSimulatePhysics(true);

    VROffset = CreateDefaultSubobject<USceneComponent>(TEXT("VROffset"));
    VROffset->SetupAttachment(GetRootComponent());

    TeleportationIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportationIndicator"));
    TeleportationIndicator->SetupAttachment(VROffset);


    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(VROffset);

    LeftMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
    LeftMotionControllerComponent->SetupAttachment(VROffset);
    LeftMotionControllerComponent->MotionSource = FName("Left");
    LeftMotionControllerComponent->SetCollisionProfileName("NoCollision");
    LeftMotionControllerComponent->SetGenerateOverlapEvents(false);

    LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
    LeftHandMesh->SetupAttachment(LeftMotionControllerComponent);

    LeftGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftGrabSphere"));
    LeftGrabSphere->SetupAttachment(LeftHandMesh);
    LeftGrabSphere->SetSphereRadius(7.0f);
    LeftGrabSphere->ShapeColor = FColor::Red;
    LeftGrabSphere->SetCollisionObjectType(ECC_WorldDynamic);
    LeftGrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LeftGrabSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    LeftGrabSphere->SetCollisionResponseToChannel(ECC_Grabbable, ECR_Overlap);

    LeftArcDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArcDirection"));
    LeftArcDirection->SetupAttachment(LeftHandMesh);

    RightMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
    RightMotionControllerComponent->SetupAttachment(VROffset);
    RightMotionControllerComponent->MotionSource = FName("Right");
    RightMotionControllerComponent->SetCollisionProfileName("NoCollision");
    RightMotionControllerComponent->SetGenerateOverlapEvents(false);

    RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
    RightHandMesh->SetupAttachment(RightMotionControllerComponent);

    RightGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightGrabSphere"));
    RightGrabSphere->SetupAttachment(RightHandMesh);
    RightGrabSphere->SetSphereRadius(7.0f);
    RightGrabSphere->ShapeColor = FColor::Blue;
    RightGrabSphere->SetCollisionObjectType(ECC_WorldDynamic);
    RightGrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RightGrabSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    RightGrabSphere->SetCollisionResponseToChannel(ECC_Grabbable, ECR_Overlap);

    RightArcDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArcDirection"));
    RightArcDirection->SetupAttachment(RightHandMesh);

    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    ActorsToIgnore.Add(nullptr);
}

void AHammeringPawn::BeginPlay()
{
    Super::BeginPlay();

    if (IsShowingDebug)
    {
        RightGrabSphere->SetHiddenInGame(false);
        LeftGrabSphere->SetHiddenInGame(false);
    }

    TeleportationIndicator->SetVisibility(false);
}

void AHammeringPawn::OnGrabRight()
{
    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("Grabing Right"));
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("Grabing Right"));
        }
    }

    AActor* NearestPickup = GetNearestOverlappingPickup(RightGrabSphere);

    RightAttachedPickup = Cast<IPickupInterface>(NearestPickup);
    if (RightAttachedPickup != nullptr)
    {
        RightAttachedPickup->GrabPressed(RightGrabSphere);
        RightGripStat = GripClose;

        if (RightAttachedPickup == LeftAttachedPickup)
        {
            LeftAttachedPickup = nullptr;
            LeftGripStat = GripOpen;
        }
    }
}

void AHammeringPawn::OnGrabLeft()
{
    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("Grabing Left"));
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("Grabing Left"));
        }
    }

    AActor* NearestPickup = GetNearestOverlappingPickup(LeftGrabSphere);

    LeftAttachedPickup = Cast<IPickupInterface>(NearestPickup);
    if (LeftAttachedPickup != nullptr)
    {
        LeftAttachedPickup->GrabPressed(LeftGrabSphere);
        LeftGripStat = GripClose;

        if (RightAttachedPickup == LeftAttachedPickup)
        {
            RightAttachedPickup = nullptr;
            RightGripStat = GripOpen;
        }
    }
}

void AHammeringPawn::OnReleaseRight()
{
    if (RightAttachedPickup != nullptr)
    {
        RightAttachedPickup->GrabReleased();
        RightAttachedPickup = nullptr;
        RightGripStat = GripOpen;
    }
}

void AHammeringPawn::OnReleaseLeft()
{
    if (LeftAttachedPickup != nullptr)
    {
        LeftAttachedPickup->GrabReleased();
        LeftAttachedPickup = nullptr;
        LeftGripStat = GripOpen;
    }
}

void AHammeringPawn::OnPressTeleportRight()
{
    bIsUpdatingTeleportDestination = true;
    bIsRightHandDoTeleportation = true;
}


void AHammeringPawn::OnReleaseTeleportRight()
{
    if (!bIsDestinationFound) { return; }

    UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(
        0, 1, TeleportFadeDelay, FColor::Black, false, true);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHammeringPawn::BeginTeleport, TeleportFadeDelay);

    bIsUpdatingTeleportDestination = false;
    TeleportationIndicator->SetVisibility(false);
    bIsDestinationFound = false;
}

void AHammeringPawn::OnPressTeleportLeft()
{
}

void AHammeringPawn::OnReleaseTeleportLeft()
{
}

void AHammeringPawn::UpdateDestinationMarker()
{
    auto ActiveHand = bIsRightHandDoTeleportation ? RightHandMesh : LeftHandMesh;

    const FVector StartPos = ActiveHand->GetComponentLocation() + ActiveHand->GetForwardVector() * TeleportBeginOffset;
    const FVector EndPos = StartPos + ActiveHand->GetForwardVector() * MaxTeleportDistance;
    FHitResult HitResult;

    bIsHitTeleportTarget = GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility);

    FNavLocation PointOnNavMeshLocation;
    const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    const auto bIsProjectedToNavMesh = NavSystem->ProjectPointToNavigation(HitResult.Location, PointOnNavMeshLocation);

    if (bIsHitTeleportTarget && bIsProjectedToNavMesh)
    {
        TeleportationIndicator->SetWorldLocation(PointOnNavMeshLocation.Location);

        auto ForwardVector = ActiveHand->GetForwardVector();
        ForwardVector = FVector(ForwardVector.X, ForwardVector.Y, 0);

        TeleportationIndicator->SetWorldRotation(ForwardVector.Rotation());
        TeleportationIndicator->SetVisibility(true);
        bIsDestinationFound = true;
    }
    else
    {
        TeleportationIndicator->SetVisibility(false);
        bIsDestinationFound = false;
    }
}

void AHammeringPawn::BeginTeleport()
{
    GetRootComponent()->SetWorldLocation(
        TeleportationIndicator->GetComponentLocation() + PawnBody->GetScaledCapsuleHalfHeight() * PawnBody->
        GetUpVector());

    UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(1, 0, TeleportFadeDelay, FColor::Black);
}

void AHammeringPawn::GrabAxisRight(const float AxisValue)
{
    if (!bIsTrackingRightAxis) { return; }

    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("Axis Value %f"), AxisValue);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue,
                                             FString::Printf(TEXT("Right Axis Value %f"), AxisValue));
        }
    }

    RightGripStat = AxisValue;
}

void AHammeringPawn::GrabAxisLeft(const float AxisValue)
{
    if (!bIsTrackingLeftAxis) { return; }

    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("Axis Value %f"), AxisValue);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red,
                                             FString::Printf(TEXT("Left Axis Value %f"), AxisValue));
        }
    }

    LeftGripStat = AxisValue;
}

AActor* AHammeringPawn::GetNearestOverlappingPickup(USphereComponent* SphereComponent)
{
    AActor* NearestPickup = nullptr;
    TArray<AActor*> OverlappingActors;

    SphereComponent->GetOverlappingActors(OverlappingActors);

    float ShortestDistance = TNumericLimits<float>::Max();
    for (auto Actor : OverlappingActors)
    {
        if (Actor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
        {
            const auto Distance = FVector::Distance(SphereComponent->GetComponentLocation(), Actor->GetActorLocation());
            if (Distance < ShortestDistance)
            {
                ShortestDistance = Distance;
                NearestPickup = Actor;
            }
        }
    }

    return NearestPickup;
}

void AHammeringPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsUpdatingTeleportDestination)
    {
        UpdateDestinationMarker();
    }
}

void AHammeringPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &AHammeringPawn::OnGrabRight);
    PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &AHammeringPawn::OnGrabLeft);
    PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &AHammeringPawn::OnReleaseRight);
    PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &AHammeringPawn::OnReleaseLeft);

    PlayerInputComponent->BindAxis("GrabAxisRight", this, &AHammeringPawn::GrabAxisRight);
    PlayerInputComponent->BindAxis("GrabAxisLeft", this, &AHammeringPawn::GrabAxisLeft);

    PlayerInputComponent->BindAction("TeleportRight", IE_Pressed, this, &AHammeringPawn::OnPressTeleportRight);
    PlayerInputComponent->BindAction("TeleportRight", IE_Released, this, &AHammeringPawn::OnReleaseTeleportRight);
    PlayerInputComponent->BindAction("TeleportLeft", IE_Pressed, this, &AHammeringPawn::OnPressTeleportLeft);
    PlayerInputComponent->BindAction("TeleportLeft", IE_Released, this, &AHammeringPawn::OnPressTeleportLeft);
}

int AHammeringPawn::GetTypeOfGrab(const bool bIsRightHanded) const
{
    if (bIsRightHanded)
    {
        if (RightAttachedPickup != nullptr) { return RightAttachedPickup->GetGrabType(); }
        return EGrabType::Controller;
    }

    if (LeftAttachedPickup != nullptr) { return LeftAttachedPickup->GetGrabType(); }
    return EGrabType::Controller;
}
