// Fill out your copyright notice in the Description page of Project Settings.


#include "HandAnimInstance.h"

void FHandAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
    Super::PreUpdate(InAnimInstance, DeltaSeconds);

    auto AnimInstance = Cast<UHandAnimInstance>(InAnimInstance);
    if (IsValid(AnimInstance))
    {
        if (IsValid(AnimInstance->Character))
        {
            PosingProgress = AnimInstance->Character->GetPoseProgress(AnimInstance->HandType);
            TypeOfGrab = AnimInstance->Character->GetTypeOfGrab(AnimInstance->HandType);
        }
    }
}

void UHandAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Character = Cast<AFPCharacter>(TryGetPawnOwner());
    if (Character != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Black, FString::Printf(TEXT("Character Name: %s"), *Character->GetName()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Black, FString::Printf(TEXT("NO Character is foudn")));
    }
}
