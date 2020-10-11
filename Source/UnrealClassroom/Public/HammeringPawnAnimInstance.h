#pragma once

#include "CoreMinimal.h"

#include "HammeringPawn.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"

#include "HammeringPawnAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FHammeringPawnAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	
	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere)
	float Grip = 0.0f;
};

UCLASS()
class UNREALCLASSROOM_API UHammeringPawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FHammeringPawnAnimInstanceProxy Proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &Proxy;
	}

	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditDefaultsOnly)
	bool bIsRightHanded = false;
	
	UPROPERTY(Transient)
	AHammeringPawn* Pawn;

	friend struct FHammeringPawnAnimInstanceProxy;
	
};
