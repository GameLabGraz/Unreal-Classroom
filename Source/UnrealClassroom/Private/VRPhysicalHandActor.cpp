#include "VRPhysicalHandActor.h"

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
#include "PickupInterface.h"

AVRPhysicalHandActor::AVRPhysicalHandActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
    SetRootComponent(SceneRootComponent);

    MotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
    MotionControllerComponent->SetupAttachment(SceneRootComponent);
    MotionControllerComponent->MotionSource = FName("Left");
    MotionControllerComponent->SetCollisionProfileName("NoCollision");
    MotionControllerComponent->SetGenerateOverlapEvents(false);

    HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
    HandMesh->SetupAttachment(MotionControllerComponent);

    GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
    GrabSphere->SetupAttachment(HandMesh);
    GrabSphere->SetSphereRadius(7.0f);
    GrabSphere->ShapeColor = FColor::Red;
    GrabSphere->SetCollisionObjectType(ECC_WorldDynamic);
    GrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GrabSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    GrabSphere->SetCollisionResponseToChannel(ECC_Grabbable, ECR_Overlap);

    ProjectileSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ProjectilePath"));
    ProjectileSpline->SetupAttachment(HandMesh);

    ArcDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ArcDirection"));
    ArcDirection->SetupAttachment(HandMesh);

    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    ActorsToIgnore.Add(nullptr);
}

void AVRPhysicalHandActor::GrabAxis(float AxisValue)
{
    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("%s Axis Value %f"), *GetName(), AxisValue);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue,
                                             FString::Printf(TEXT("%s Axis Value %f"), *GetName(), AxisValue));
        }
    }

    GripState = AxisValue;
}

AActor* AVRPhysicalHandActor::OnGrab()
{
    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("Grab %s"), *GetName());
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red,
                                             FString::Printf(TEXT("Grab %s"), *GetName()));
        }
    }

    AActor* NearestPickup = GetNearestOverlappingPickup();
    AttachedPickup = Cast<IPickupInterface>(NearestPickup);

    if(AttachedPickup !=nullptr)
    {
        AttachedPickup->GrabPressed(GrabSphere);
    }
    
    return NearestPickup;
}

void AVRPhysicalHandActor::OnRelease()
{
    if(AttachedPickup != nullptr)
    {
        AttachedPickup->GrabReleased();
        AttachedPickup = nullptr;
    }
}

void AVRPhysicalHandActor::OnPressTeleport()
{
}

void AVRPhysicalHandActor::OnReleaseTeleport()
{
}

AActor* AVRPhysicalHandActor::GetNearestOverlappingPickup() const
{
    AActor* NearestPickup = nullptr;
    TArray<AActor*> OverlappingActors;

    GrabSphere->GetOverlappingActors(OverlappingActors);

    float ShortestDistance = TNumericLimits<float>::Max();
    for (auto Actor : OverlappingActors)
    {
        if (Actor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
        {
            const auto Distance = FVector::Distance(GrabSphere->GetComponentLocation(), Actor->GetActorLocation());
            if (Distance < ShortestDistance)
            {
                ShortestDistance = Distance;
                NearestPickup = Actor;
            }
        }
    }

    return NearestPickup;
}

void AVRPhysicalHandActor::BeginPlay()
{
    Super::BeginPlay();

    if (IsShowingDebug)
    {
        GrabSphere->SetHiddenInGame(false);
    }
}

void AVRPhysicalHandActor::Tick(float DeltaSeconds)
{
}
