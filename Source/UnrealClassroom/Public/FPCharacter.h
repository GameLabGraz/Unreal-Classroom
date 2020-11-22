#pragma once

#include "CoreMinimal.h"


#include "HandSkeletalMeshComponent.h"
#include "GameFramework/Character.h"

#include "FPCharacter.generated.h"

class USphereComponent;
class UHandSkeletalMeshComponent;
class UCameraComponent;

UCLASS()
class UNREALCLASSROOM_API AFPCharacter : public ACharacter
{
    GENERATED_BODY()


    // Methods:
public:
    AFPCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void InteractionRightPressed();
    void InteractionRightReleased();

    void InteractionLeftPressed();
    void InteractionLeftReleased();

    void MoveForward(float Value);
    void MoveRight(float Value);

    void TurnAtRate(float Value);
    void LookupAtRate(float Value);

    void GrabRight(float Value);
    void GrabLeft(float Value);

    void GrabRaycast();
    AActor* FindNearestGrabbableActor(USphereComponent* HandGrabSphere);

    void ResetHand(EHandType HandType);
    void ReAttachHand(EHandType HandType);

    float GetPoseProgress(EHandType HandType);
    int GetTypeOfGrab(EHandType HandType);

protected:
    virtual void BeginPlay() override;

private:


    // Members:	
public:

    UPROPERTY(EditAnywhere, Category = "[Property]: Debug")
    bool IsDebuggingController = false;
    
    UPROPERTY(EditAnywhere, Category = "[Property]: Components")
    UCameraComponent* FPCamera;

    UPROPERTY(EditAnywhere, Category = "[Property]: Components")
    UHandSkeletalMeshComponent* RightHandComponent;

    UPROPERTY(EditAnywhere, Category = "[Property]: Components")
    UHandSkeletalMeshComponent* LeftHandComponent;

    UPROPERTY(EditAnywhere, Category = "[Property]: LocoMotion")
    float TurnBaseRate = 45;

    UPROPERTY(EditAnywhere, Category = "[Property]: LocoMotion")
    float LookupBaseRate = 45;

    UPROPERTY(EditAnywhere, Category = "[Property]: Grab")
    float MaxGrabDistance = 100;

    UPROPERTY(EditAnywhere, Category = "[Property]: Grab")
    float HandDimension = 20;
    

    private:
    FVector InitialRelativeLocationRightHand;
    FVector InitialRelativeLocationLeftHand;
    FRotator InitialRelativeRotationRightHand;
    FRotator InitialRelativeRotationLeftHand;

    float RightPoseProgress;
    float LeftPoseProgress;

    FHitResult GrabRaycastResult;
    bool IsGrabRaycastHit = false;

    bool bIsReadyForGrabRight = false;
    bool bIsReadyForGrabLeft = false;

    UPROPERTY()
    AActor* SuspectActorForGrab = nullptr;

    UPROPERTY(Transient)
    AActor* RightGrabbedActor = nullptr;
    
    UPROPERTY(Transient)
    AActor* LeftGrabbedActor = nullptr;
};


