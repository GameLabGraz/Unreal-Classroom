#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

UENUM()
enum EGrabType
{
	Controller UMETA(DisplayName = "Controller"),
	Box UMETA(DisplayName = "Box"),
	Bow UMETA(DisplayName = "Bow"),
	Sphere UMETA(DisplayName = "Sphere"), 
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALCLASSROOM_API IPickupInterface
{
	GENERATED_BODY()

public:
	virtual void GrabPressed(USceneComponent* AttachTo) {}

	virtual void GrabReleased() {}

	virtual int GetGrabType() {return 0;}
};
