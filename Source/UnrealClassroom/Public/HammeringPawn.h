// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "PickupInterface.h"
#include "GameFramework/Pawn.h"
#include "HammeringPawn.generated.h"


class UMotionControllerComponent;
class USphereComponent;
class UCameraComponent;


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

    void OnGrabRight();
    void OnGrabLeft();
    void OnReleaseRight();
    void OnReleaseLeft();

    void GrabAxisRight(float AxisValue);

    UPROPERTY(EditAnywhere, Category = "Debuging")
    bool IsShowingDebug = true;

    IPickupInterface* LeftAttachedPickup;
    IPickupInterface* RightAttachedPickup;

    static AActor* GetNearestOverlappingPickup(USphereComponent* SphereComponent);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region components

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* RightHandMesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* RightGrabSphere;

    UPROPERTY(VisibleAnywhere)
    UMotionControllerComponent* RightMotionControllerComponent;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* LeftHandMesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* LeftGrabSphere;

    UPROPERTY(VisibleAnywhere)
    UMotionControllerComponent* LeftMotionControllerComponent;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* VROffset;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootSceneComponent;

#pragma endregion
};
