#include "HammeringPawn.h"

#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "HammeringStatics.h"


AHammeringPawn::AHammeringPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    SetRootComponent(RootSceneComponent);

    VROffset = CreateDefaultSubobject<USceneComponent>(TEXT("VROffset"));
    VROffset->SetupAttachment(GetRootComponent());

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
}

void AHammeringPawn::BeginPlay()
{
    Super::BeginPlay();

    if (IsShowingDebug)
    {
        RightGrabSphere->SetHiddenInGame(false);
        LeftGrabSphere->SetHiddenInGame(false);
    }
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

        if(RightAttachedPickup == LeftAttachedPickup)
        { 
            LeftAttachedPickup = nullptr;
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

        if(RightAttachedPickup == LeftAttachedPickup)
        {
            RightAttachedPickup = nullptr;
        }
    }
}

void AHammeringPawn::OnReleaseRight()
{
    if(RightAttachedPickup != nullptr)
    {
        RightAttachedPickup->GrabReleased();
        RightAttachedPickup = nullptr;
    }
}

void AHammeringPawn::OnReleaseLeft()
{
    if (LeftAttachedPickup != nullptr)
    {
        LeftAttachedPickup->GrabReleased();
        LeftAttachedPickup = nullptr;
    }
}

void AHammeringPawn::GrabAxisRight(float AxisValue)
{
    if (IsShowingDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("Axis Value %f"), AxisValue);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, FString::Printf(TEXT("Axis Value %f"), AxisValue));
        }
    }
}

AActor* AHammeringPawn::GetNearestOverlappingPickup(USphereComponent* SphereComponent)
{
    AActor* NearestPickup = nullptr;
    TArray<AActor*> OverlappingActors;
    
    SphereComponent->GetOverlappingActors(OverlappingActors);

    float ShortestDistance = TNumericLimits<float>::Max();
    for (auto Actor : OverlappingActors)
    {
        if(Actor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
        {
            const auto Distance = FVector::Distance(SphereComponent->GetComponentLocation(), Actor->GetActorLocation());
            if(Distance < ShortestDistance)
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
}

void AHammeringPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &AHammeringPawn::OnGrabRight);
    PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &AHammeringPawn::OnGrabLeft);
    PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &AHammeringPawn::OnReleaseRight);
    PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &AHammeringPawn::OnReleaseLeft);

    PlayerInputComponent->BindAxis("GrabAxisRight", this, &AHammeringPawn::GrabAxisRight);
}
