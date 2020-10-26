#pragma once

#include "PickupActor.h"
#include "PickupInterface.h"
#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"


class AVRPhysicalHandActor;
class UVRPhysicalHandComponent;

struct FPredictProjectilePathResult;
struct FPredictProjectilePathParams;

class UCapsuleComponent;
class USplineComponent;
class USplineMeshComponent;
class UMotionControllerComponent;
class USphereComponent;
class UCameraComponent;
class UArrowComponent;

UCLASS()
class UNREALCLASSROOM_API AVRPawn : public APawn
{
    GENERATED_BODY()

public:
    
    AVRPawn();

protected:
    
    virtual void BeginPlay() override;
    
    void SpawnHandActors();

    void GrabAxisRight(float AxisValue);
    void GrabAxisLeft(float AxisValue);

    void OnGrabRight();
    void OnGrabLeft();
    void OnReleaseRight();
    void OnReleaseLeft();

    void OnPressTeleportRight();
    void OnReleaseTeleportRight();
    void OnPressTeleportLeft();
    void OnReleaseTeleportLeft();

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

    AVRPhysicalHandActor* RightHand = nullptr;
    AVRPhysicalHandActor* LeftHand = nullptr;

    IPickupInterface* LeftAttachedPickup;
    IPickupInterface* RightAttachedPickup;

public:
    

#pragma region component

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* VROffset;

    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* PawnBody;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AVRPhysicalHandActor> RightController;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AVRPhysicalHandActor> LeftController;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* TeleportationIndicator;

#pragma endregion
};
