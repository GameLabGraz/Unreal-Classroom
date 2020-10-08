#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

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
};
