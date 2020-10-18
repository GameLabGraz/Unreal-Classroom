#pragma once

#include "CoreMinimal.h"


#include "PickupActor.h"
#include "PickupInterface.h"
#include "GameFramework/Pawn.h"
#include "HammeringPawn.generated.h"


struct FPredictProjectilePathResult;
struct FPredictProjectilePathParams;

class UCapsuleComponent;
class USplineComponent;
class USplineMeshComponent;
class UMotionControllerComponent;
class USphereComponent;
class UCameraComponent;
class UArrowComponent;

constexpr float GripOpen = 0.0f;
constexpr float GripClose = 1.0f;

UCLASS()
class UNREALCLASSROOM_API AHammeringPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AHammeringPawn();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    bool bIsTrackingRightAxis = true;
    bool bIsTrackingLeftAxis = true;

    bool bIsHitTeleportTarget = false;
    bool bIsUpdatingTeleportDestination = false;
    bool bIsDestinationFound = false;
    bool bIsRightHandDoTeleportation = true;

    void OnGrabRight();
    void OnGrabLeft();
    void OnReleaseRight();
    void OnReleaseLeft();

    void OnPressTeleportRight();
    void OnReleaseTeleportRight();
    void OnPressTeleportLeft();
    void OnReleaseTeleportLeft();

    void UpdateDestinationMarker();
    bool UpdateProjectilePath(FPredictProjectilePathParams& PathParams, FPredictProjectilePathResult& PathResult,
                              FVector StartPos, FVector LaunchVelocityVector) const;
    void UpdateProjectileSpline(TArray<FVector> PathPoints) const;
    void ClearProjectilePool();
    void UpdateProjectileMesh(TArray<FVector> PathPoints);
    void BeginTeleport() const;

    float LeftGripStat = 0.0f;
    float RightGripStat = 0.0f;

    void GrabAxisRight(float AxisValue);
    void GrabAxisLeft(float AxisValue);

    IPickupInterface* LeftAttachedPickup;
    IPickupInterface* RightAttachedPickup;

    static AActor* GetNearestOverlappingPickup(USphereComponent* SphereComponent);

    FRotator InitialRotationOfController = FRotator::ZeroRotator;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FORCEINLINE float GetGripStat(const bool bIsRightHanded) { return bIsRightHanded ? RightGripStat : LeftGripStat; }
    int GetTypeOfGrab(const bool bIsRightHanded) const;

protected:
    UPROPERTY(EditAnywhere, Category = "Debuging")
    bool IsShowingDebug = true;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float MaxTeleportDistance = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float TeleportBeginOffset = 20.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float TeleportFadeDelay = 1.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float ProjectileDuration = 10.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float TeleportLaunchVelocity = 900.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float ProjectilePathRadius = 5.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    bool bIsTraceComplex = true;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    bool bIsShowProjectionDebug = true;

    UPROPERTY(EditDefaultsOnly, Category = "[Properties]: Teleportation")
    UStaticMesh* TeleportProjectileMesh;

    UPROPERTY(EditDefaultsOnly, Category = "[Properties]: Teleportation")
    UMaterialInterface* TeleportProjectileMaterial;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    TArray<AActor*> ActorsToIgnore;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    FVector ProjectNavigationExtend = FVector(500.0f, 500.0f, 500.0f);

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    UHapticFeedbackEffect_Base* TeleportationHaptic;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    float TeleportHapticIntensity = 0.3f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
    FColor CameraFadeColor = FColor::Black;

    UPROPERTY(VisibleAnywhere)
    TArray<class USplineMeshComponent*> ProjectileMeshPool;

    UPROPERTY()
    int RightTempAnimType;

    UPROPERTY()
    int LeftTempAnimType;

#pragma region components

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* RightHandMesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* RightGrabSphere;

    UPROPERTY(VisibleAnywhere)
    UArrowComponent* RightArcDirection;

    UPROPERTY(VisibleAnywhere)
    UMotionControllerComponent* RightMotionControllerComponent;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* LeftHandMesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* LeftGrabSphere;

    UPROPERTY(VisibleAnywhere)
    UArrowComponent* LeftArcDirection;

    UPROPERTY(VisibleAnywhere)
    UMotionControllerComponent* LeftMotionControllerComponent;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* VROffset;

    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* PawnBody;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* TeleportationIndicator;

    UPROPERTY(VisibleAnywhere)
    USplineComponent* RightProjectileSpline;

    UPROPERTY(VisibleAnywhere)
    USplineComponent* LeftProjectileSpline;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* TestMesh;
#pragma endregion
};