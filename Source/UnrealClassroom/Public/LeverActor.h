// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GrabbableInterface.h"
#include "GameFramework/Actor.h"
#include "MovieScene/Public/MovieSceneSequencePlayer.h"

#include "LeverActor.generated.h"

class USphereComponent;


UCLASS()
class UNREALCLASSROOM_API ALeverActor : public AActor, public IGrabbableInterface
{
	GENERATED_BODY()

	// Methods
public:	
	ALeverActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GrabPressed(USceneComponent* AttachTo) override;
	virtual void GrabReleased() override;
	virtual int GetGrabType() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "[Property]: Animation")
	void PlayAnimation(bool IsActive);

	protected:
	virtual void BeginPlay() override;

	// Members
	public:

	UPROPERTY(EditAnywhere, Category = "[Property]: Debug")
	bool IsDebuggingGrab = false;

	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	UStaticMeshComponent* BaseMeshComponent;

	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	UStaticMeshComponent* LeverMeshComponent;

	UPROPERTY(EditAnywhere, Category = "[Property]: Components")
	USphereComponent* CustomAttachPoint;

	UPROPERTY(EditAnywhere, Category = "[Property]: AttachBehaviour")
	bool bSnapHandToMesh;

	UPROPERTY(EditAnywhere, Category = "[Property]: AttachBehaviour")
	TEnumAsByte<EGrabType> TypeOfGrab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[Property]: Function")
	bool IsLeverInActiveMode = false;

};
