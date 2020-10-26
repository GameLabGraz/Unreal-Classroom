#include "HammeringPawnAnimInstance.h"

void FHammeringPawnAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, const float DeltaSeconds)
{
    Super::PreUpdate(InAnimInstance, DeltaSeconds);

    UHammeringPawnAnimInstance* AnimInstance = Cast<UHammeringPawnAnimInstance>(InAnimInstance);
    if (IsValid(AnimInstance))
    {
        if (IsValid(AnimInstance->Pawn))
        {
            Grip = AnimInstance->Pawn->GetGripStat(AnimInstance->bIsRightHanded);
            TypeOfGrab = AnimInstance->Pawn->GetTypeOfGrab(AnimInstance->bIsRightHanded);
        }
    }
}

void UHammeringPawnAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Pawn = Cast<AHammeringPawn>(TryGetPawnOwner());
}
