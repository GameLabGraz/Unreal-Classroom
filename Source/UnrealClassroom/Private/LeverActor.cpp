// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverActor.h"

#include "UnrealClassRoomStatics.h"
#include "Components/SphereComponent.h"


// Sets default values
ALeverActor::ALeverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	SetRootComponent(BaseMeshComponent);
	BaseMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BaseMeshComponent->SetSimulatePhysics(false);

	LeverMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("LeverMeshComponent");
	LeverMeshComponent->SetupAttachment(BaseMeshComponent);
	LeverMeshComponent->SetCollisionObjectType(ECC_Grabbable);
	LeverMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeverMeshComponent->SetSimulatePhysics(false);

	CustomAttachPoint = CreateDefaultSubobject<USphereComponent>(TEXT("CustomAttachPoint"));
	CustomAttachPoint->SetupAttachment(LeverMeshComponent);
	CustomAttachPoint->SetSphereRadius(10.0f);
	CustomAttachPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
	CustomAttachPoint->ShapeColor = FColor::Emerald;
	CustomAttachPoint->SetHiddenInGame(false);

}

// Called when the game starts or when spawned
void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverActor::GrabPressed(USceneComponent* AttachTo)
{
	PlayAnimation(IsLeverInActiveMode);
	IsLeverInActiveMode = !IsLeverInActiveMode;
}

void ALeverActor::GrabReleased()
{
}

int ALeverActor::GetGrabType()
{
	return TypeOfGrab;
}

