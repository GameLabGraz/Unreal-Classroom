#pragma once

#include "CoreMinimal.h"

#include "PickupInterface.h"
#include "GameFramework/Actor.h"
#include "LeverHandleActor.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS()
class UNREALCLASSROOM_API ALeverHandleActor : public AActor, public IPickupInterface
{
    GENERATED_BODY()

public:
    ALeverHandleActor();

    bool bIsTicking = false;

    UFUNCTION(BlueprintCallable)
    virtual void GrabPressed(USceneComponent* AttachTo) override;
    
    UFUNCTION(BlueprintCallable)
    virtual void GrabReleased() override;

    virtual int GetGrabType() override;

protected:
    virtual void BeginPlay() override;

    FRotator InitialAttachPointRotation;
    FRotator LastStillRotationAttachPoint;

    FVector GetCustomAttachLocation() const;
    FRotator GetCustomAttachRotation() const;

    UFUNCTION()
    void OnAttachPointBeingOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    void CheckForDetachAfterMaximumRotation();

public:
    virtual void Tick(float DeltaTime) override;


protected:

    UPROPERTY(EditAnywhere, Category = "[Properties]: Handle Behaviour")
    bool bIsDebugHandle = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Handle Behaviour")
    float MaxRotation = 45.0f;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Handle Behaviour")
    bool bHasStillPosition = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Handle Behaviour")
    float MinRotationGapForStillStage = 5;

    UPROPERTY(VisibleAnywhere, Category = "[Properties]: Attach Behaviour")
    USphereComponent* CustomAttachPoint;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bSnapToHand = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bUseCustomRotation = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bUseCustomLocation = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Attach Behaviour")
    bool bIsShowingDebug = false;

    UPROPERTY(EditAnywhere, Category = "[Properties]: Animation")
    TEnumAsByte<EGrabType> TypeOfGrab;

#pragma region component

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* HandleAttachPoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* HandleMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent* PhysicsConstraintMain;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent* PhysicsConstraintVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPhysicsConstraintComponent* PhysicsConstraintGrab;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneRootComponent;
    

#pragma endregion
};
