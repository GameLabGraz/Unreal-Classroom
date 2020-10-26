#pragma once

#include "CoreMinimal.h"

#include "PickupInterface.h"
#include "GameFramework/Actor.h"
#include "VRPhysicalHandActor.generated.h"

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
class UNREALCLASSROOM_API AVRPhysicalHandActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AVRPhysicalHandActor();

	void GrabAxis(float AxisValue);
	
	AActor* OnGrab();
	void OnRelease();

	void OnPressTeleport();
	void OnReleaseTeleport();

	AActor* GetNearestOverlappingPickup() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	

protected:

	UPROPERTY(EditAnywhere, Category = "Debuging")
	bool IsShowingDebug = true;

	UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, Category = "[Properties]: Teleportation")
	TArray<AActor*> ActorsToIgnore;


	IPickupInterface* AttachedPickup;
	

public:

	bool bIsTrackingAxis = true;

	float GripState = 0.0f;

#pragma region component

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRootComponent;

	UPROPERTY(EditAnywhere)
	UMotionControllerComponent* MotionControllerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* HandMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* GrabSphere;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArcDirection;

	UPROPERTY(EditAnywhere)
	USplineComponent* ProjectileSpline;

#pragma endregion 

	
};
