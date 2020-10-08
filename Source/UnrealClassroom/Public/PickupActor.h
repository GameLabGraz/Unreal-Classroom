#pragma once

#include "CoreMinimal.h"

#include "PickupInterface.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS()
class UNREALCLASSROOM_API APickupActor : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	APickupActor();

	virtual void GrabPressed(USceneComponent* AttachTo) override;
	virtual void GrabReleased() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

};
