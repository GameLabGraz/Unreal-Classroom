#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "FPCHandComponent.generated.h"


class USphereComponent;


UCLASS( ClassGroup=(CharacterComponents), meta=(BlueprintSpawnableComponent) )
class UNREALCLASSROOM_API UFPCHandComponent : public USceneComponent
{
	GENERATED_BODY()

	// Methods:
public:	
	UFPCHandComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

	// Members:
public:
	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	USkeletalMeshComponent* HandMeshComponent;

	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	USphereComponent* GrabSphereComponent;

		
};
