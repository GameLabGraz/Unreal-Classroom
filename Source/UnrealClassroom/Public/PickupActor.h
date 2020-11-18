#pragma once

#include "CoreMinimal.h"

#include "GrabbableInterface.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class USphereComponent;
UCLASS()
class UNREALCLASSROOM_API APickupActor : public AActor, public IGrabbableInterface
{
	GENERATED_BODY()

	// Methods
public:	
	APickupActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GrabPressed(USceneComponent* AttachTo) override;
	virtual void GrabReleased() override;
	virtual int GetGrabType() override;

protected:
	virtual void BeginPlay() override;

	// Members
public:

	UPROPERTY(EditAnywhere, Category = "[Property]: Debug")
	bool IsDebuggingGrab = false;

	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	USphereComponent* CustomAttachPoint;

	UPROPERTY(EditAnywhere, Category = "[Property]: AttachBehaviour")
	bool bSnapHandToMesh;

	UPROPERTY(EditAnywhere, Category = "[Property]: AttachBehaviour")
	TEnumAsByte<EGrabType> TypeOfGrab;


};
