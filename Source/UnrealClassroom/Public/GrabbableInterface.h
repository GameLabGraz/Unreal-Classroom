#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrabbableInterface.generated.h"

UENUM()
enum EGrabType
{
    Controller UMETA(DisplayName = "Controller"),
    Box UMETA(DisplayName = "Box"),
    Bow UMETA(DisplayName = "Bow"),
    Sphere UMETA(DisplayName = "Sphere"),
    Pointing UMETA(DisplayName = "Pointing"),
    Lever UMETA(DisplayName = "Lever"),
};

UINTERFACE(MinimalAPI)
class UGrabbableInterface : public UInterface
{
    GENERATED_BODY()
};

class UNREALCLASSROOM_API IGrabbableInterface
{
    GENERATED_BODY()

public:

    virtual void GrabPressed(USceneComponent* AttachTo){}
    virtual void GrabReleased(){}
    virtual int GetGrabType(){return 0;}
};
