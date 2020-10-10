#include "HandPawnAnimInstance.h"

void FHandPawnAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
    UHandPawnAnimInstance* Instance = Cast<UHandPawnAnimInstance>(InAnimInstance);

    if(IsValid(Instance))
    {
        if(IsValid(Instance->Pawn))
        {
            Grip = Instance->Pawn->GetGripStat(Instance->bIsRightHanded);
        }
    }
}

void UHandPawnAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Pawn = Cast<AHammeringPawn>(TryGetPawnOwner());
}
