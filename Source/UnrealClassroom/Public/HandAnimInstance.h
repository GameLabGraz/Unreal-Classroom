#pragma once

#include "CoreMinimal.h"

#include "FPCharacter.h"
#include "HandSkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"

#include "HandAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FHandAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "[Property]: Inputs")
	float PosingProgress = 0.0f;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "[Property]: Inputs")
	int TypeOfGrab = 0;
	
};


UCLASS()
class UNREALCLASSROOM_API UHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FHandAnimInstanceProxy GripProxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &GripProxy;
	}

	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EHandType> HandType = RightHand;

	UPROPERTY(Transient)
	AFPCharacter* Character;

	friend struct FHandAnimInstanceProxy;
};
